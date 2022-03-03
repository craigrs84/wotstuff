using Microsoft.AspNetCore.Mvc;
using System.Globalization;
using wotdata.Models;
using CsvHelper;

namespace data.Controllers;

[ApiController]
[Route("[controller]")]
public class StatsController : ControllerBase
{
    private readonly ILogger<StatsController> _logger;
    private AppDbContext _dbContext;

    public StatsController(
        ILogger<StatsController> logger,
        AppDbContext dbContext)
    {
        _logger = logger;
        _dbContext = dbContext;
    }

    [HttpPost]
    public void Post(Stat stat)
    {
        _dbContext.Stats.Add(stat);
        _dbContext.SaveChanges();
    }

    [HttpGet]
    public IEnumerable<Stat> Get(string sex, string race, string @class, string homeland, int? level = 3)
    {
        var stats = QueryStats(sex, race, @class, homeland, level);
        return stats;
    }

    [HttpGet]
    [Route("csv")]
    public async Task GetCsv(string sex, string race, string @class, string homeland, int? level = 3)
    {
        var stats = QueryStats(sex, race, @class, homeland, level);

        Response.StatusCode = 200;
        Response.ContentType = "text/csv";
        Response.Headers["Content-Disposition"] = "attachment; filename=\"Stats.csv\"";

        await using (var writer = new StreamWriter(Response.Body))
        await using (var csv = new CsvWriter(writer, CultureInfo.InvariantCulture))
        {
            await csv.WriteRecordsAsync(stats);
            await csv.FlushAsync();
        }
    }

    private IQueryable<Stat> QueryStats(string sex, string race, string @class, string homeland, int? level)
    {
        sex = sex?.Trim()?.ToLower();
        race = race?.Trim()?.ToLower();
        @class = @class?.Trim()?.ToLower();
        homeland = homeland?.Trim()?.ToLower();

        var query = _dbContext.Stats.AsQueryable();
        if (!string.IsNullOrWhiteSpace(sex))
        {
            query = query.Where(x => x.Sex.ToLower() == sex);
        }
        if (!string.IsNullOrWhiteSpace(race))
        {
            query = query.Where(x => x.Race.ToLower() == race);
        }
        if (!string.IsNullOrWhiteSpace(@class))
        {
            query = query.Where(x => x.Class.ToLower() == @class);
        }
        if (!string.IsNullOrWhiteSpace(homeland))
        {
            query = query.Where(x => x.Homeland.ToLower() == homeland);
        }
        if (level > 0)
        {
            query = query.Where(x => x.Level == level);
        }

        query = query.OrderByDescending(x => x.Id);
        return query;
    }
}
