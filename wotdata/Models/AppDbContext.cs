using System;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;

namespace wotdata.Models;

public class AppDbContext : DbContext
{
    public DbSet<Weapon> Weapons { get; set; }
    public DbSet<Wearable> Wearables { get; set; }

    public string DbPath { get; }

    protected override void OnConfiguring(DbContextOptionsBuilder options)
    {
        options.UseSqlite($"Data Source=wotdata.db");
    }

    protected override void OnModelCreating(ModelBuilder builder) {
        builder.Entity<Weapon>().ToTable("Weapons");
        builder.Entity<Wearable>().ToTable("Wearables");
    }
}