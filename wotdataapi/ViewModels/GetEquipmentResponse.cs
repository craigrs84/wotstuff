using wotdata.Models;

namespace wotdata.ViewModels;

public class GetEquipmentResponse
{
    public IEnumerable<Weapon> Weapons { get; set; }
    public IEnumerable<Wearable> Wearables { get; set; }
}