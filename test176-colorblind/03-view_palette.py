import sys

import colorspacious
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

def main():
    norm_palette = np.loadtxt(sys.stdin)

    jch = colorspacious.cspace_convert(norm_palette, 'sRGB1', 'JCh')
    norm_palette = norm_palette[np.argsort(jch[:, 2])]

    deut_palette = colorblind(norm_palette, 'deuteranomaly')
    prot_palette = colorblind(norm_palette, 'protanomaly')
    trit_palette = colorblind(norm_palette, 'tritanomaly')
    gray_palette = grayscale(norm_palette)

    fig, axes = plt.subplots(5, 1, figsize=(6, 4), tight_layout=True, dpi=96)
    show_palette(axes[0], norm_palette).set_title('Normal')
    show_palette(axes[1], deut_palette).set_title('Deuteranomaly')
    show_palette(axes[2], prot_palette).set_title('Protanomaly')
    show_palette(axes[3], trit_palette).set_title('Tritanomaly')
    show_palette(axes[4], gray_palette).set_title('Grayscale')
    plt.show()

def colorblind(colors, type):
    space = {
        'name': 'sRGB1+CVD',
        'cvd_type': type,
        'severity': 100,
    }
    return np.clip(colorspacious.cspace_convert(colors, space, 'sRGB1'), 0, 1)

def grayscale(colors):
    jch = colorspacious.cspace_convert(colors, 'sRGB1', 'JCh')
    jch[:, 1] = 0
    return np.clip(colorspacious.cspace_convert(jch, 'JCh', 'sRGB1'), 0, 1)

def show_palette(ax, palette):
    cmap = mpl.colors.LinearSegmentedColormap.from_list('', palette)
    ax.matshow(np.arange(len(palette)).reshape(1, -1),
               vmin=0, vmax=16, cmap=cmap, aspect='auto')
    ax.tick_params(axis='both', which='both',
                   bottom='off', labelbottom='off',
                   top='off', labeltop='off',
                   left='off', labelleft='off',
                   right='off', labelright='off')
    return ax

if __name__ == '__main__':
    main()
