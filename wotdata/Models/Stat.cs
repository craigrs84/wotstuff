using System;

namespace wotdata.Models;

public class Stat
{
    public int Id { get; set; }
    public string SessionId { get; set; }
    public int Played { get; set; }
    public int Age { get; set; }
    public string Sex { get; set; }
    public string Race { get; set; }
    public string Class { get; set; }
    public string Homeland { get; set; }
    public int Level { get; set; }
    public int Hps { get; set; }
    public int Mvs { get; set; }
    public int Height { get; set; }
    public int Weight { get; set; }
    public int Strength { get; set; }
    public int Intelligence { get; set; }
    public int Willpower { get; set; }
    public int Dexterity { get; set; }
    public int Constitution { get; set; }
    public double Bmi
    {
        get
        {
            return Height > 0 ? Math.Round((double)Weight / Height / Height * 10, 6) : 0;
        }        
    }
}
