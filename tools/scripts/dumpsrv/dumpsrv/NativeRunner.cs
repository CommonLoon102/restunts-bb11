using System.Diagnostics;
using System.Globalization;

namespace DumpSrv;

public sealed record NativeInvocation(string GameDirectory, string Executable,
    string ReplayBaseName, string Arguments, int TimeoutSeconds, int? OraclePspSegment = null);

public interface INativeRunner
{
    Task<ProcessResult> RunAsync(NativeInvocation invocation, CancellationToken cancellationToken);
}

public sealed class NativeRunner : INativeRunner
{
    public Task<ProcessResult> RunAsync(NativeInvocation invocation,
        CancellationToken cancellationToken) => ProcessRunner.RunAsync(CreateStartInfo(invocation),
            invocation.TimeoutSeconds, cancellationToken);

    public static ProcessStartInfo CreateStartInfo(NativeInvocation invocation)
    {
        ReplayCatalog.ValidateBaseName(invocation.ReplayBaseName);
        var startInfo = new ProcessStartInfo
        {
            FileName = Path.GetFullPath(invocation.Executable),
            WorkingDirectory = Path.GetFullPath(invocation.GameDirectory),
            UseShellExecute = false,
            CreateNoWindow = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true
        };
        startInfo.ArgumentList.Add(invocation.ReplayBaseName);
        foreach (var argument in invocation.Arguments.Split(' ', StringSplitOptions.RemoveEmptyEntries))
        {
            startInfo.ArgumentList.Add(argument);
        }
        startInfo.Environment["SDL_VIDEODRIVER"] = "dummy";
        startInfo.Environment["SDL_AUDIODRIVER"] = "dummy";
        startInfo.Environment["RESTUNTS_ORACLE_PSP_SEGMENT"] =
            (invocation.OraclePspSegment ?? 654).ToString(CultureInfo.InvariantCulture);
        startInfo.Environment["RESTUNTS_ORACLE_PROGRAM_PATH"] = @"C:\PIXLDUMP.EXE";
        return startInfo;
    }
}
