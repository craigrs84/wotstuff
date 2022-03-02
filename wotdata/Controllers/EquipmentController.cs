using Mapster;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using wotdata.Models;
using wotdata.ViewModels;

namespace wotdata.Controllers;

[ApiController]
[Route("[controller]")]
public class EquipmentController : ControllerBase
{
    private readonly ILogger<EquipmentController> _logger;
    private AppDbContext _db;

    public EquipmentController(ILogger<EquipmentController> logger, AppDbContext db)
    {
        _logger = logger;
        _db = db;
    }

    [HttpGet(Name = "GetEquipment")]
    public GetEquipmentResponse GetEquipment()
    {
        var weapons = _db.Weapons.AsNoTracking().Adapt<List<WeaponDetail>>();
        var wearables = _db.Wearables.AsNoTracking();

        return new GetEquipmentResponse {
            Weapons = weapons,
            Wearables = wearables
        };
    }

    [HttpPost("weapons", Name = "AddWeapon")]
    public ActionResult<Weapon> UpdateWeapon(Weapon model)
    {
        model.Id = 0;
        _db.Add(model);
        _db.SaveChanges();
        return model;
    }

    [HttpPut("weapons/{id}", Name = "UpdateWeapon")]
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

    [HttpDelete("weapons/{id}", Name = "DeleteWeapon")]
    public void DeleteWeapon(int id)
    {
        var item = _db.Weapons.Find(id);
        if (item != null)
        {
            _db.Weapons.Remove(item);
            _db.SaveChanges();
        }
    }

    [HttpPost("wearables", Name = "AddWearable")]
    public ActionResult<Wearable> AddWearable(Wearable model)
    {
        model.Id = 0;
        _db.Add(model);
        _db.SaveChanges();
        return model;
    }

    [HttpPut("wearables/{id}", Name = "UpdateWearable")]
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

    [HttpDelete("wearables/{id}", Name = "DeleteWearable")]
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
