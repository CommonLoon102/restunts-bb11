namespace DumpSrv;

public class RegressionEngine(IDosBoxRunner? runner = null, Action<string>? log = null,
    TimeProvider? timeProvider = null, INativeRunner? nativeRunner = null)
{
    private readonly IDosBoxRunner runner = runner ?? new DosBoxRunner();
    private readonly INativeRunner nativeRunner = nativeRunner ?? new NativeRunner();
    private readonly Action<string> log = log ?? Console.WriteLine;

    private sealed record Phase(bool Renderer, string Oracle, string Candidate, string OracleExtension,
        string CandidateExtension, string Arguments, int TimeoutSeconds);

    public virtual async Task<ShardResult> RunAsync(RunOptions options, CancellationToken cancellationToken)
    {
        var result = new ShardResult
        {
            CandidatePlatform = options.CandidatePlatform,
            OraclePspSegment = options.CandidatePlatform == CandidatePlatforms.Sdl3
                ? options.OraclePspSegment ?? NativeRunner.DefaultOraclePspSegment : null,
            ShardIndex = options.ShardIndex,
            ShardCount = options.ShardCount,
            PartitionCount = options.PartitionCount,
            PhysicsTests = options.PhysicsTests,
            RendererTests = options.RendererTests,
            RendererTestPercentage = options.RendererTestPercentage,
            Camera = options.Camera,
            Target = options.Target,
            DosBoxTimeoutSeconds = options.DosBoxTimeoutSeconds,
            RendererTimeoutSeconds = options.RendererTimeoutSeconds
        };
        var gate = new object();
        void Diagnostic(string message)
        {
            lock (gate)
            {
                result.Diagnostics.Add(message);
            }
            log(message);
        }
        void Own(string path)
        {
            lock (gate)
            {
                result.OwnedFiles.Add(path);
            }
        }
        try
        {
            Validate(options);
            var replays = ReplayCatalog.Discover(options.GameDirectory, cancellationToken);
            result.ReplayFiles = replays.ToList();
            var rendererReplays = ReplayCatalog.RendererReplays(options.GameDirectory, replays,
                options.Target, cancellationToken);
            var plan = ShardPlan.Load(options.ShardPlanPath, replays, rendererReplays,
                options.RendererTestPercentage, options.ShardCount, options.Target);
            var phases = new List<Phase>();
            if (options.PhysicsTests)
            {
                phases.Add(new Phase(false, "repldumo.exe", "repldump.exe", "BIN", "BNI", "1",
                    options.DosBoxTimeoutSeconds));
            }
            if (options.RendererTests)
            {
                phases.Add(new Phase(true, "pixldumo.exe", "pixldump.exe", "PDO", "PDD",
                    $"{options.Camera} {options.Target}", options.RendererTimeoutSeconds));
            }
            foreach (var phase in phases)
            {
                var oracle = DosFiles.Resolve(options.GameDirectory, phase.Oracle);
                var candidate = CandidatePath(options, phase.Candidate);
                if (!File.Exists(oracle))
                {
                    throw new FileNotFoundException($"DOS oracle executable not found: {oracle}");
                }
                if (!File.Exists(candidate))
                {
                    throw new FileNotFoundException($"Candidate executable not found: {candidate}");
                }
            }
            Directory.CreateDirectory(options.OutputDirectory);
            foreach (var phase in phases)
            {
                cancellationToken.ThrowIfCancellationRequested();
                var timer = new ProcessingTimer(timeProvider);
                timer.Start();
                try
                {
                    var assigned = plan.Assigned(phase.Renderer, options.ShardIndex);
                    var partitions = ReplayCatalog.RoundRobin(assigned, options.PartitionCount);
                    await Task.WhenAll(partitions.Where(partition => partition.Count > 0).Select(async partition =>
                    {
                        foreach (var replay in partition)
                        {
                            cancellationToken.ThrowIfCancellationRequested();
                            log($"Processing {(phase.Renderer ? "renderer" : "physics")} replay: {replay}");
                            try
                            {
                                await ProcessReplayAsync(options, replay, phase, Diagnostic, Own, cancellationToken);
                            }
                            catch (OperationCanceledException) when (cancellationToken.IsCancellationRequested)
                            {
                                throw;
                            }
                            catch (Exception exception) when (exception is not OutOfMemoryException)
                            {
                                Diagnostic($"ERROR|type=processing_failure|input={replay}|message={ReportFormatter.Safe(exception.Message)}");
                            }
                            lock (gate)
                            {
                                (phase.Renderer ? result.RendererCompleted : result.PhysicsCompleted).Add(replay);
                            }
                        }
                    }));
                }
                finally
                {
                    timer.Stop();
                    if (phase.Renderer)
                    {
                        result.RendererElapsed = timer.Elapsed;
                    }
                    else
                    {
                        result.PhysicsElapsed = timer.Elapsed;
                    }
                }
            }
            result.Completed = true;
        }
        catch (OperationCanceledException) when (cancellationToken.IsCancellationRequested)
        {
            result.Failure = "Regression processing was cancelled.";
            Diagnostic("ERROR|type=cancelled|message=Regression processing was cancelled.");
        }
        catch (Exception exception) when (exception is not OutOfMemoryException)
        {
            result.Failure = exception.Message;
            Diagnostic($"ERROR|type=processing_failure|message={ReportFormatter.Safe(exception.Message)}");
        }
        result.PhysicsCompleted.Sort(StringComparer.Ordinal);
        result.RendererCompleted.Sort(StringComparer.Ordinal);
        result.Diagnostics = ReportFormatter.Lines(result.Diagnostics).ToList();
        return result;
    }

    private async Task ProcessReplayAsync(RunOptions options, string replay, Phase phase,
        Action<string> diagnostic, Action<string> own, CancellationToken cancellationToken)
    {
        var basename = Path.GetFileNameWithoutExtension(replay);
        string Resolve(string extension) => DosFiles.Resolve(options.GameDirectory, $"{basename}.{extension}");
        var candidatePath = Resolve(phase.CandidateExtension);
        own(candidatePath);
        File.Delete(candidatePath);
        var frames = await DumpOutput.ReadFrameCountAsync(
            Path.Combine(options.GameDirectory, replay), cancellationToken);
        var oraclePath = Resolve(phase.OracleExtension);
        var pendingPath = Resolve($"{phase.OracleExtension}.pending");
        var settingsPath = phase.Renderer ? Resolve("PDO.settings") : null;
        var settingsMatch = settingsPath is null || (File.Exists(settingsPath) &&
            await File.ReadAllTextAsync(settingsPath, cancellationToken) == phase.Arguments);
        if (!settingsMatch || File.Exists(pendingPath) ||
            !await DumpOutput.IsCompleteAsync(oraclePath, phase.Renderer, frames,
                cancellationToken))
        {
            own(pendingPath);
            await File.WriteAllTextAsync(pendingPath, "", cancellationToken);
            File.Delete(oraclePath);
            if (settingsPath is not null)
            {
                File.Delete(settingsPath);
            }
            if (!await ExecuteAsync(phase.Oracle))
            {
                return;
            }
            oraclePath = Resolve(phase.OracleExtension);
            if (!await ValidateOutputAsync(oraclePath))
            {
                return;
            }
            if (settingsPath is not null)
            {
                await File.WriteAllTextAsync(settingsPath, phase.Arguments, cancellationToken);
            }
            File.Delete(pendingPath);
        }
        if (!await ExecuteAsync(phase.Candidate))
        {
            return;
        }
        // DOS may create upper-case paths even for lower-case host replay names.
        candidatePath = Resolve(phase.CandidateExtension);
        own(candidatePath);
        if (!await ValidateOutputAsync(candidatePath))
        {
            return;
        }
        if (!await FilesEqualAsync(oraclePath, candidatePath, cancellationToken))
        {
            diagnostic($"ERROR|type=file_mismatch|input={replay}|" +
                $"{phase.OracleExtension.ToLowerInvariant()}={Path.GetFileName(oraclePath)}|" +
                $"{phase.CandidateExtension.ToLowerInvariant()}={Path.GetFileName(candidatePath)}");
        }
        return;

        async Task<bool> ValidateOutputAsync(string path)
        {
            if (!File.Exists(path))
            {
                diagnostic($"ERROR|type=missing_output|input={replay}|" +
                    $"output={Path.GetFileName(path)}");
                return false;
            }
            if (!await DumpOutput.IsCompleteAsync(path, phase.Renderer, frames, cancellationToken))
            {
                diagnostic($"ERROR|type=invalid_output|input={replay}|" +
                    $"output={Path.GetFileName(path)}|expected_frames={frames}");
                return false;
            }
            return true;
        }

        async Task<bool> ExecuteAsync(string executable)
        {
            var native = executable == phase.Candidate &&
                options.CandidatePlatform == CandidatePlatforms.Sdl3;
            var outcome = native
                ? await nativeRunner.RunAsync(new NativeInvocation(options.GameDirectory,
                    CandidatePath(options, executable), basename, phase.Arguments,
                    phase.TimeoutSeconds, options.OraclePspSegment), cancellationToken)
                : await runner.RunAsync(new DosBoxInvocation(options.GameDirectory,
                    options.DosBoxConfigPath, executable, basename, phase.Arguments,
                    phase.TimeoutSeconds), cancellationToken);
            if (outcome.TimedOut)
            {
                diagnostic($"ERROR|type=timeout|exe={executable}|input={replay}|timeout_seconds={phase.TimeoutSeconds}");
            }
            else if (!outcome.Success)
            {
                var detail = outcome.ExitCode.HasValue ? $"exit_code={outcome.ExitCode}" :
                    $"message={ReportFormatter.Safe(outcome.Failure ?? "Executable failed.")}";
                var failureType = native ? "native_failure" : "dosbox_failure";
                diagnostic($"ERROR|type={failureType}|exe={executable}|input={replay}|{detail}");
            }
            // Track partially written candidates too, including different DOS host casing.
            own(Resolve(phase.CandidateExtension));
            return outcome.Success;
        }
    }

    public static void Cleanup(ShardResult result)
    {
        foreach (var path in result.OwnedFiles.Distinct(StringComparer.Ordinal))
        {
            if (path.EndsWith(".pending", StringComparison.OrdinalIgnoreCase) && File.Exists(path))
            {
                var incompletePath = DosFiles.Resolve(Path.GetDirectoryName(path)!,
                    Path.GetFileName(path)[..^".pending".Length]);
                File.Delete(incompletePath);
                if (incompletePath.EndsWith(".PDO", StringComparison.OrdinalIgnoreCase))
                {
                    File.Delete(DosFiles.Resolve(Path.GetDirectoryName(path)!,
                        Path.GetFileName(incompletePath) + ".settings"));
                }
            }
            File.Delete(path);
        }
    }

    private static async Task<bool> FilesEqualAsync(string left, string right, CancellationToken cancellationToken)
    {
        await using var leftStream = File.OpenRead(left);
        await using var rightStream = File.OpenRead(right);
        if (leftStream.Length != rightStream.Length)
        {
            return false;
        }
        var leftBuffer = new byte[65536];
        var rightBuffer = new byte[65536];
        while (true)
        {
            var leftCount = await leftStream.ReadAtLeastAsync(leftBuffer, leftBuffer.Length, false, cancellationToken);
            var rightCount = await rightStream.ReadAtLeastAsync(rightBuffer, rightBuffer.Length, false, cancellationToken);
            if (leftCount != rightCount || !leftBuffer.AsSpan(0, leftCount).SequenceEqual(rightBuffer.AsSpan(0, rightCount)))
            {
                return false;
            }
            if (leftCount == 0)
            {
                return true;
            }
        }
    }

    private static string CandidatePath(RunOptions options, string executable) =>
        options.CandidatePlatform == CandidatePlatforms.Sdl3
            ? Path.Combine(options.NativeDirectory!, Path.GetFileNameWithoutExtension(executable) +
                (OperatingSystem.IsWindows() ? ".exe" : ""))
            : DosFiles.Resolve(options.GameDirectory, executable);

    private static void Validate(RunOptions options)
    {
        CandidatePlatforms.Validate(options.CandidatePlatform);
        if (options.CandidatePlatform == CandidatePlatforms.Sdl3 &&
            (string.IsNullOrWhiteSpace(options.NativeDirectory) ||
                !Path.IsPathFullyQualified(options.NativeDirectory)))
        {
            throw new ArgumentException(
                "NativeDirectory must be an absolute path for SDL3 candidates.");
        }
        if (options.CandidatePlatform == CandidatePlatforms.Dos &&
            (options.NativeDirectory is not null || options.OraclePspSegment is not null))
        {
            throw new ArgumentException(
                "NativeDirectory and OraclePspSegment require SDL3 candidates.");
        }
        if (options.OraclePspSegment is < 1 or > ushort.MaxValue)
        {
            throw new ArgumentOutOfRangeException(nameof(options.OraclePspSegment));
        }
        ArgumentOutOfRangeException.ThrowIfLessThan(options.Camera, RendererSettings.MinimumCamera);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(options.Camera,
            RendererSettings.MaximumCamera);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.Target, RendererSettings.PlayerTarget);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(options.Target,
            RendererSettings.OpponentTarget);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.PartitionCount, 1);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(options.PartitionCount, 64);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.ShardCount, 1);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.ShardIndex, 0);
        ArgumentOutOfRangeException.ThrowIfGreaterThanOrEqual(options.ShardIndex, options.ShardCount);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.RendererTestPercentage, 1);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(options.RendererTestPercentage, 100);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.DosBoxTimeoutSeconds, 1);
        ArgumentOutOfRangeException.ThrowIfLessThan(options.RendererTimeoutSeconds, 1);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(options.DosBoxTimeoutSeconds, 2147483);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(options.RendererTimeoutSeconds, 2147483);
        if (!options.PhysicsTests && !options.RendererTests)
        {
            throw new ArgumentException("At least one test phase must be enabled.");
        }
        if (!File.Exists(options.DosBoxConfigPath))
        {
            throw new FileNotFoundException("DOSBox configuration not found.", options.DosBoxConfigPath);
        }
    }

}
