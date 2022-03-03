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
}
