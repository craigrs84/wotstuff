using wotdata.Models;

namespace wotdata.ViewModels;

public class GetEquipmentResponse
{
    public IEnumerable<WeaponDetail> Weapons { get; set; }
    public IEnumerable<Wearable> Wearables { get; set; }
}