namespace wotdata.Logic;

using wotdata.Models;
using wotdata.ViewModels;

public class Calcs
{
    public static int DamageMin(Weapon m)
    {
        return m.DamageDice;
    }

    public static int DamageMax(Weapon m)
    {
        return m.DamageDice * m.DamageSides;
    }

    public static int MountedMin(Weapon m)
    {
        return MountedDice(m);
    }

    public static int MountedMax(Weapon m)
    {
        return MountedDice(m) * m.DamageSides;
    }

    public static int ChargeMin(Weapon m)
    {
        if (!m.Charge) return 0;
        return (MountedDice(m) + 2) * 7;
    }

    public static int ChargeMax(Weapon m)
    {
        if (!m.Charge) return 0;
        return (MountedDice(m) * m.DamageSides + 8) * 11;
    }

    public static int StabMin(Weapon m)
    {
        if (!m.Stab) return 0;
        return (m.DamageDice + 2) * 10;
    }

    public static int StabMax(Weapon m)
    {
        if (!m.Stab) return 0;
        return (m.DamageDice * m.DamageSides + 8) * 15;
    }

    public static int ThrowMin(Weapon m)
    {
        if (!m.Throw) return 0;
        return ThrowDice(m);
    }

    public static int ThrowMax(Weapon m)
    {
        if (!m.Throw) return 0;
        return ThrowDice(m) * m.DamageSides + 16;
    }

    public static int MountedDice(Weapon m)
    {
        return m.Type == "Spear" || m.Type == "Lance" ? m.DamageDice + 1 : m.DamageDice;
    }

    public static int ThrowDice(Weapon m)
    {
        return m.Type == "Javelin" || m.Type == "Projectile" ? 4 * m.DamageDice : 2 * m.DamageDice;
    }
}