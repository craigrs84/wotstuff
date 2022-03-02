using Microsoft.AspNetCore.HttpOverrides;
using wotdata.Models;

const string basePath = "/api/data";

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddCors();

builder.Services.AddControllers().AddJsonOptions(options => {
    options.JsonSerializerOptions.WriteIndented = true;
});

// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c => {
    c.AddServer(new Microsoft.OpenApi.Models.OpenApiServer { Url = basePath });
});

builder.Services.AddDbContext<AppDbContext>();

var app = builder.Build();
app.UsePathBase(basePath);

app.UseForwardedHeaders(new ForwardedHeadersOptions
{
    ForwardedHeaders = ForwardedHeaders.XForwardedFor | ForwardedHeaders.XForwardedProto
});

app.UseCors(x => x.AllowAnyMethod().AllowAnyHeader().AllowAnyOrigin());

// Configure the HTTP request pipeline.
app.UseSwagger();
app.UseSwaggerUI(c =>
{
    c.RoutePrefix = "swagger";
    c.SwaggerEndpoint("v1/swagger.json", "My API V1");
});

//app.UseHttpsRedirection();

app.UseAuthorization();

app.UseRouting();

app.MapControllers();

app.Run();
