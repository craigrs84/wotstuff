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
public class WeaponsController : ControllerBase
{
    private readonly ILogger<EquipmentController> _logger;
    private AppDbContext _db;

    public WeaponsController(ILogger<EquipmentController> logger, AppDbContext db)
    {
        _logger = logger;
        _db = db;
    }

    [HttpGet(Name = "GetWeapons")]
    public List<WeaponDetail> GetWeapons()
    {
        var items = _db.Weapons.AsNoTracking().Adapt<List<WeaponDetail>>();
        return items;
    }

    [HttpGet("csv", Name = "GetWeaponsCsv")]
    public async Task GetWeaponsCsv()
    {
        var items = _db.Weapons.AsNoTracking().Adapt<List<WeaponDetail>>();

        Response.StatusCode = 200;
        Response.ContentType = "text/csv";
        Response.Headers["Content-Disposition"] = "attachment; filename=\"Weapons.csv\"";

        await using (var writer = new StreamWriter(Response.Body))
        await using (var csv = new CsvWriter(writer, CultureInfo.InvariantCulture))
        {
            await csv.WriteRecordsAsync(items);
            await csv.FlushAsync();
        }
    }

    [HttpPost(Name = "AddWeapon")]
    public ActionResult<Weapon> UpdateWeapon(Weapon model)
    {
        model.Id = 0;
        _db.Add(model);
        _db.SaveChanges();
        return model;
    }

    [HttpPut("{id}", Name = "UpdateWeapon")]
    public ActionResult<Weapon> UpdateWeapon(int id, Weapon model)
    {
        model.Id = id;
        if (_db.Weapons.AsNoTracking().Any(x => x.Id == id))
        {
            _db.Attach(model).State = EntityState.Modified;
            _db.SaveChanges();
            return model;
        }

        return NotFound();
    }

    [HttpDelete("{id}", Name = "DeleteWeapon")]
    public void DeleteWeapon(int id)
    {
        var item = _db.Weapons.Find(id);
        if (item != null)
        {
            _db.Weapons.Remove(item);
            _db.SaveChanges();
        }
    }
}