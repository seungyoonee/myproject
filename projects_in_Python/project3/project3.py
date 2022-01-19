import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import math

# this makes image look better on a macbook pro
def imageshow(img, dpi=200):
    if dpi > 0:
        F = plt.gcf()
        F.set_dpi(dpi)
    plt.imshow(img)

def rgb_ints_example():
    '''should produce red,purple,green squares
    on the diagonal, over a black background'''
    # RGB indexes
    red,green,blue = range(3)
    # img array 
    # all zeros = black pixels
    # shape: (150 rows, 150 cols, 3 colors)
    img = np.zeros((150,150,3), dtype=np.uint8)
    for x in range(50):
        for y in range(50):
            # red pixels
            img[x,y,red] = 255
            # purple pixels
            # set all 3 color components
            img[x+50, y+50,:] = (128, 0, 128)
            # green pixels
            img[x+100,y+100,green] = 255
    return img

def onechannel(pattern, rgb):
    # copying the image
    img = np.copy(pattern)
    
    # red
    if rgb == 0:
        img[:,:,[1,2]] = 0
    # green
    elif rgb == 1:
        img[:,:,[0,2]] = 0
    # blue
    elif rgb == 2:
        img[:,:,[0,1]] = 0
        
    return img

def permutecolorchannels(img, perm):
    # copying the image and each red, green, and blue of perm by index
    img_copy = np.copy(img)
    red = perm[0]
    green = perm[1]
    blue = perm[2]
    
    # copying each red, green, and blue row of the image
    red_copy = img_copy[:,:,0].copy()
    green_copy = img_copy[:,:,1].copy()
    blue_copy = img_copy[:,:,2].copy()
    
    if red != 0:
        # red to green
        if red == 1:
            img_copy[:,:,1] = red_copy
        # red to blue
        elif red == 2:
            img_copy[:,:,2] = red_copy
    
    if green != 1:
        # green to red
        if green == 0:
            img_copy[:,:,0] = green_copy
        # green to blue
        elif green == 2:
            img_copy[:,:,2] = green_copy
    
    if blue != 2:
        # blue to red
        if blue == 0:
            img_copy[:,:,0] = blue_copy
        # blue to green
        elif blue == 1:
            img_copy[:,:,1] = blue_copy
                   
    return img_copy

def decrypt(image, key):
    # copying the image and key
    img = np.copy(image)
    key_copy = np.copy(key)
    
    # copying each red, green, and blue row of the img
    red = img[:,:,0].copy()
    green = img[:,:,1].copy()
    blue = img[:,:,2].copy()
    
    # XORing each red, green, and blue row of the img with the key
    img[:,:,0] = red^key_copy
    img[:,:,1] = green^key_copy
    img[:,:,2] = blue^key_copy
    
    return img
''' ========== main ========== '''
#plt.imshow(rgb_ints_example())
pattern = plt.imread("pattern.png")

# Part 1
plt.imshow(onechannel(pattern, 0))
plt.imshow(onechannel(pattern, 1))
plt.imshow(onechannel(pattern, 2))

# Part 2
permcolors = plt.imread('permcolors.jpg')
plt.imshow(permutecolorchannels(permcolors, [1,2,0]))

# Part 3
secret = plt.imread('secret.bmp')
key = np.load('key.npy')
plt.imshow(decrypt(secret, key))

