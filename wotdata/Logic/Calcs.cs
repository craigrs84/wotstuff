namespace wotdata.Logic;

using wotdata.Models;
using wotdata.ViewModels;

public class Calcs
{
    public static int DamageMin(WeaponDetail w)
    {
        return w.DamageDice;
    }

    public static int DamageMax(WeaponDetail w)
    {
        return w.DamageDice * w.DamageSides;
    }

    public static int MountedMin(WeaponDetail w)
    {
        return MountedDice(w);
    }

    public static int MountedMax(WeaponDetail w)
    {
        return MountedDice(w) * w.DamageSides;
    }

    public static int ChargeMin(WeaponDetail w)
    {
        if (!w.Charge) return 0;
        return (MountedDice(w) + 2) * 7;
    }

    public static int ChargeMax(WeaponDetail w)
    {
        if (!w.Charge) return 0;
        return (MountedDice(w) * w.DamageSides + 8) * 11;
    }

    public static int StabMin(WeaponDetail w)
    {
        if (!w.Stab) return 0;
        return (w.DamageDice + 2) * 10;
    }

    public static int StabMax(WeaponDetail w)
    {
        if (!w.Stab) return 0;
        return (w.DamageDice * w.DamageSides + 8) * 15;
    }

    public static int ThrowMin(WeaponDetail w)
    {
        if (!w.Throw) return 0;
        return ThrowDice(w);
    }

    public static int ThrowMax(WeaponDetail w)
    {
        if (!w.Throw) return 0;
        return ThrowDice(w) * w.DamageSides + 16;
    }

    public static int MountedDice(WeaponDetail w)
    {
        return w.Type == "Spear" || w.Type == "Lance" ? w.DamageDice + 1 : w.DamageDice;
    }

    public static int ThrowDice(WeaponDetail w)
    {
        return w.Type == "Javelin" || w.Type == "Projectile" ? 4 * w.DamageDice : 2 * w.DamageDice;
    }
}