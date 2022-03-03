using System.Globalization;
using CsvHelper;
using Mapster;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using wotdata.Models;
using wotdata.ViewModels;

namespace wotdata.Controllers;

[ApiController]
[Route("[controller]")]
public class WearablesController : ControllerBase
{
    private readonly ILogger<EquipmentController> _logger;
    private AppDbContext _db;

    public WearablesController(ILogger<EquipmentController> logger, AppDbContext db)
    {
        _logger = logger;
        _db = db;
    }

    [HttpGet(Name = "GetWearables")]
    public List<Wearable> GetWearables()
    {
        var items = _db.Wearables.AsNoTracking().ToList();
        return items;
    }

    [HttpGet("csv", Name = "GetWearablesCsv")]
    public async Task GetWearablesCsv()
    {
        var items = _db.Wearables.AsNoTracking().ToList();

        Response.StatusCode = 200;
        Response.ContentType = "text/csv";
        Response.Headers["Content-Disposition"] = "attachment; filename=\"Wearables.csv\"";

        await using (var writer = new StreamWriter(Response.Body))
        await using (var csv = new CsvWriter(writer, CultureInfo.InvariantCulture))
        {
            await csv.WriteRecordsAsync(items);
            await csv.FlushAsync();
        }
    }

    [HttpPost(Name = "AddWearable")]
    public ActionResult<Wearable> AddWearable(Wearable model)
    {
        model.Id = 0;
        _db.Add(model);
        _db.SaveChanges();
        return model;
    }

    [HttpPut("{id}", Name = "UpdateWearable")]
    public ActionResult<Wearable> UpdateWearable(int id, Wearable model)
    {
        model.Id = id;
        if (!_db.Wearables.AsNoTracking().Any(x => x.Id == id))
        {
            _db.Attach(model).State = EntityState.Modified;
            _db.SaveChanges();
            return model;
        }

        return NotFound();
    }

    [HttpDelete("{id}", Name = "DeleteWearable")]
    public void DeleteWearable(int id)
    {
        var item = _db.Wearables.Find(id);
        if (item != null)
        {
            _db.Wearables.Remove(item);
            _db.SaveChanges();
        }
    }
}