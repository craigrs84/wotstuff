namespace wotdata.Models;

public class Wearable
{
    public int Id { get; set; }
    public string Name { get; set; }
    public string Type { get; set; }
    public string Style { get; set; }
    public string Quality { get; set; }
    public string Sheath { get; set; }
    public bool NoLocate { get; set; }
    public bool NoRent { get; set; }
    public decimal Weight { get; set; }
    public int Db { get; set; }
    public int Pb { get; set; }
    public int Abs { get; set; }
    public int Mv { get; set; }
}