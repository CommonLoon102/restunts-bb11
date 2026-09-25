using System.Text.Json.Serialization;

namespace DumpSrv;

public static class CandidatePlatforms
{
    public const string Dos = "dos";
    public const string Sdl3 = "sdl3";

    public static string Validate(string platform)
    {
        if (platform is not Dos and not Sdl3)
        {
            throw new ArgumentException("CandidatePlatform must be dos or sdl3.");
        }
        return platform;
    }
}

public sealed record RunOptions
{
    public string CandidatePlatform { get; init; } = CandidatePlatforms.Dos;
    public string? NativeDirectory { get; init; }
    public int? OraclePspSegment { get; init; }
    public required string GameDirectory { get; init; }
    public required string OutputDirectory { get; init; }
    public string DosBoxConfigPath { get; init; } = Path.Combine(AppContext.BaseDirectory, "dosbox.proc.conf");
    public int PartitionCount { get; init; } = 1;
    public string? ShardPlanPath { get; init; }
    public int ShardIndex { get; init; }
    public int ShardCount { get; init; } = 1;
    public bool PhysicsTests { get; init; } = true;
    public bool RendererTests { get; init; } = true;
    public int RendererTestPercentage { get; init; } = 100;
    public int Camera { get; init; } = RendererSettings.DefaultCamera;
    public int Target { get; init; } = RendererSettings.PlayerTarget;
    public int DosBoxTimeoutSeconds { get; init; } = 60;
    public int RendererTimeoutSeconds { get; init; } = 60;
}

public sealed record ServiceOptions
{
    public required string ApiKey { get; init; }
    public required int PartitionCount { get; init; }
    public int Port { get; init; } = 8080;
    public int DosBoxTimeoutSeconds { get; init; } = 60;
    public int RendererTestPercentage { get; init; } = 100;
    public int Camera { get; init; } = RendererSettings.DefaultCamera;
    public int Target { get; init; } = RendererSettings.PlayerTarget;
    public int ResponseProcessingTimeoutSeconds { get; init; } = 1800;
    public string ServiceDirectory { get; init; } = AppContext.BaseDirectory;
}

public sealed class ShardResult
{
    [JsonRequired]
    public string CandidatePlatform { get; set; } = CandidatePlatforms.Dos;
    public int? OraclePspSegment { get; set; }
    [JsonRequired]
    public int Version { get; set; } = 1;
    [JsonRequired]
    public int ShardIndex { get; set; }
    [JsonRequired]
    public int ShardCount { get; set; }
    [JsonRequired]
    public int PartitionCount { get; set; }
    [JsonRequired]
    public bool PhysicsTests { get; set; }
    [JsonRequired]
    public bool RendererTests { get; set; }
    [JsonRequired]
    public int RendererTestPercentage { get; set; }
    [JsonRequired]
    public int Camera { get; set; } = RendererSettings.DefaultCamera;
    [JsonRequired]
    public int Target { get; set; } = RendererSettings.PlayerTarget;
    [JsonRequired]
    public int DosBoxTimeoutSeconds { get; set; } = 60;
    [JsonRequired]
    public int RendererTimeoutSeconds { get; set; } = 60;
    [JsonRequired]
    public List<string> ReplayFiles { get; set; } = [];
    [JsonRequired]
    public List<string> PhysicsCompleted { get; set; } = [];
    [JsonRequired]
    public List<string> RendererCompleted { get; set; } = [];
    [JsonRequired]
    public List<string> Diagnostics { get; set; } = [];
    [JsonRequired]
    public bool Completed { get; set; }
    [JsonRequired]
    public string? Failure { get; set; }
    [JsonIgnore]
    public TimeSpan? PhysicsElapsed { get; set; }
    [JsonIgnore]
    public TimeSpan? RendererElapsed { get; set; }
    [JsonIgnore]
    public List<string> OwnedFiles { get; set; } = [];
}
