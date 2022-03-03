using wotdata.Logic;

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
    public int DamageMin => Calcs.DamageMin(this);
    public int DamageMax => Calcs.DamageMax(this);
    public int DamageRange => DamageMax - DamageMin;
    public decimal DamageMean => (DamageMax + DamageMin) / 2m;
    public int MountedMin => Calcs.MountedMin(this);
    public int MountedMax => Calcs.MountedMax(this);
    public int MountedRange => MountedMax - MountedMin;
    public decimal MountedMean => (MountedMax + MountedMin) / 2m;
    public int ChargeMin => Calcs.ChargeMin(this);
    public int ChargeMax => Calcs.ChargeMax(this);
    public int ChargeRange => ChargeMax - ChargeMin;
    public decimal ChargeMean => (ChargeMax + ChargeMin) / 2m;
    public int StabMin => Calcs.StabMin(this);
    public int StabMax => Calcs.StabMax(this);
    public int StabRange => StabMax - StabMin;
    public decimal StabMean => (StabMax + StabMin) / 2m;    
    public int ThrowMin => Calcs.ThrowMin(this);
    public int ThrowMax => Calcs.ThrowMax(this);
    public int ThrowRange => ThrowMax - ThrowMin;
    public decimal ThrowMean => (ThrowMax + ThrowMin) / 2m;   
}