![output image](https://raw.githubusercontent.com/snsinfu/bit4/master/test163-gg/a.png)

## Notes

- Paths are anti-aliased
- Use half-integer coordinates to draw solid 1-pixel line without blur

    dc.DrawRectangle(5.5, 5.5, 40, 40)

- `gg` does not have image scaling functionality
- `gg` can export an [Image](https://golang.org/pkg/image/#Image), which then
   may be rescaled
- Here [github.com/nfnt/resize](https://github.com/nfnt/resize) is used
