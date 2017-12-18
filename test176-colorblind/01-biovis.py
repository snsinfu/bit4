import colorspacious
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

# http://mkweb.bcgsc.ca/biovis2012/
# http://mkweb.bcgsc.ca/biovis2012/color-blindness-palette.png
palette = np.array([
    (  0,   0,   0),
    (  0,  73,  73),
    (  0, 146, 146),
    (255, 109, 182),
    (255, 182, 219),
    ( 73,   0, 146),
    (  0, 109, 219),
    (182, 109, 255),
    (109, 182, 255),
    (182, 219, 255),
    (146,   0,   0),
    (146,  73,   0),
    (219, 109,   0),
    ( 36, 255,  36),
    (255, 255, 109),
])
palette = palette / 255.0

def colorblind(pal, typ):
    input_space = {
        'name':     'sRGB1+CVD',
        'cvd_type': typ,
        'severity': 100,
    }
    return np.clip(colorspacious.cspace_convert(pal, input_space, 'sRGB1'),
                   0, 1)

deuteranomaly_palette = colorblind(palette, 'deuteranomaly')
protanomaly_palette = colorblind(palette, 'protanomaly')
tritanomaly_palette = colorblind(palette, 'tritanomaly')

def show_palette(ax, pal):
    cm = mpl.colors.LinearSegmentedColormap.from_list('', pal)
    ax.matshow(np.arange(15).reshape(3, 5),
               vmin=0, vmax=14, cmap=cm, aspect='auto')
    ax.set_xticklabels([])
    ax.set_yticklabels([])
    return ax

fig, axes = plt.subplots(4, 1, figsize=(3, 7), tight_layout=True)

show_palette(axes[0], palette).set_title('Normal')
show_palette(axes[1], deuteranomaly_palette).set_title('Deuteranomaly')
show_palette(axes[2], protanomaly_palette).set_title('Protanomaly')
show_palette(axes[3], tritanomaly_palette).set_title('Tritanomaly')

plt.show()
