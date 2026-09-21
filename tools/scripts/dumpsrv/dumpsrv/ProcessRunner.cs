using System.Diagnostics;

namespace DumpSrv;

public sealed record ProcessResult(int? ExitCode = null, bool TimedOut = false, string? Failure = null)
{
    public bool Success => ExitCode == 0 && !TimedOut && Failure is null;
}

internal static class ProcessRunner
{
    public static async Task<ProcessResult> RunAsync(ProcessStartInfo startInfo, int timeoutSeconds,
        CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();
        using var process = new Process { StartInfo = startInfo };
        var started = false;
        Task output = Task.CompletedTask;
        Task error = Task.CompletedTask;
        using var timeout = CancellationTokenSource.CreateLinkedTokenSource(cancellationToken);
        timeout.CancelAfter(TimeSpan.FromSeconds(timeoutSeconds));
        try
        {
            started = process.Start();
            if (!started)
            {
                return new ProcessResult(Failure: $"{startInfo.FileName} did not start.");
            }
            // Drain without retaining unlimited child output in memory.
            output = process.StandardOutput.BaseStream.CopyToAsync(Stream.Null);
            error = process.StandardError.BaseStream.CopyToAsync(Stream.Null);
            await process.WaitForExitAsync(timeout.Token);
            return new ProcessResult(process.ExitCode);
        }
        catch (OperationCanceledException) when (!cancellationToken.IsCancellationRequested)
        {
            return new ProcessResult(TimedOut: true);
        }
        catch (OperationCanceledException)
        {
            throw;
        }
        catch (Exception exception) when (exception is not OutOfMemoryException)
        {
            return new ProcessResult(Failure: exception.Message);
        }
        finally
        {
            if (started && !process.HasExited)
            {
                // SIGKILL avoids DOSBox's shutdown confirmation window.
                try
                {
                    process.Kill(entireProcessTree: true);
                }
                catch (InvalidOperationException) when (process.HasExited)
                {
                }
                catch (System.ComponentModel.Win32Exception) when (!process.HasExited)
                {
                    // Still terminate the process if child-tree enumeration failed.
                    process.Kill();
                }
                await process.WaitForExitAsync(CancellationToken.None);
            }
            await Task.WhenAll(output, error);
        }
    }
}
