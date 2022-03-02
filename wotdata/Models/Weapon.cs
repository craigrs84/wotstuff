namespace wotdata.Models;

public class Weapon
{
    public int Id { get; set; }
    public string Name { get; set; }
    public string Type { get; set; }
    public string Style { get; set; }
    public string Quality { get; set; }
    public string Handed { get; set; }
    public bool Poison { get; set; }
    public bool Attack { get; set; }
    public bool Charge { get; set; }
    public bool Stab { get; set; }
    public bool Throw { get; set; }
    public decimal Weight { get; set; }
    public int Ob { get; set; }
    public int Pb { get; set; }
    public int DamageDice { get; set; }
    public int DamageSides { get; set; }
    public string DamageType { get; set; }
}