# osuTo2007
Convert any osu beatmap to v3 (playable in the 2007 client)

# What does it do?
- Restore v3 beatmap structure
- Handling SV points by multiplying with BPM (bc 2007 doesnt support SV) -> SV Point to Timing Point
- Fix Slider Start Position
- Represent modern slider shapes in v3
- Perfect Circle Slider handling

# Performance Issues within 2007 client
- Beatmaps with a lot of timing points/objects will take long time to load when u play<br/>
  -> each 100KB ~ 5-6minute load time<br/>
  -> ^^^^^^ ~200% File Size increase on Aspire Map<br/>
  -> lag spikes/unstable frame rate<br/>
  -> b337 currently the earliest client that is optimized enough (might still have long loading times on large maps but plays them smooth at least)

# How to run
Download from release, or compile<br/>
-> place .osu files into the same directory where the executable(windows)/binary(linux) is<br/>

