using System.Diagnostics;

namespace DumpSrv;

public sealed record DosBoxInvocation(string GameDirectory, string ConfigPath, string Executable,
    string ReplayBaseName, string Arguments, int TimeoutSeconds);

public interface IDosBoxRunner
{
    Task<ProcessResult> RunAsync(DosBoxInvocation invocation, CancellationToken cancellationToken);
}

public sealed class DosBoxRunner(string? executablePath = null) : IDosBoxRunner
{
    public async Task<ProcessResult> RunAsync(DosBoxInvocation invocation,
        CancellationToken cancellationToken)
    {
        return await ProcessRunner.RunAsync(CreateStartInfo(invocation, executablePath),
            invocation.TimeoutSeconds, cancellationToken);
    }

    public static ProcessStartInfo CreateStartInfo(DosBoxInvocation invocation, string? executablePath = null)
    {
        ReplayCatalog.ValidateBaseName(invocation.ReplayBaseName);
        var gameDirectory = Path.GetFullPath(invocation.GameDirectory);
        if (gameDirectory.IndexOfAny(['"', '\r', '\n', '%']) >= 0)
        {
            throw new InvalidDataException("The game directory contains unsupported DOS command characters.");
        }
        var configuredPath = executablePath ?? Environment.GetEnvironmentVariable("DUMPSRV_DOSBOX_PATH");
        if (string.IsNullOrWhiteSpace(configuredPath))
        {
            const string windowsPath = @"C:\DOSBox-x\dosbox-X.exe";
            configuredPath = OperatingSystem.IsWindows() && File.Exists(windowsPath) ? windowsPath : "dosbox-x";
        }
        var startInfo = new ProcessStartInfo
        {
            FileName = configuredPath,
            WorkingDirectory = gameDirectory,
            UseShellExecute = false,
            CreateNoWindow = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true
        };
        string[] arguments =
        [
            "-silent", "-conf", Path.GetFullPath(invocation.ConfigPath),
            "-noautoexec", "-set", "cpu core=dynamic", "-set", "cpu cycles=max",
            "-c", $"mount c \"{gameDirectory}\"", "-c", "c:",
            "-c", $"{invocation.Executable} \"{invocation.ReplayBaseName}\" {invocation.Arguments}",
            "-c", "exit"
        ];
        foreach (var argument in arguments)
        {
            startInfo.ArgumentList.Add(argument);
        }
        return startInfo;
    }
}
