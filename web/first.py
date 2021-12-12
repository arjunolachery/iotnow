DICELossDict = {} # dictionary to store the DICE Loss of all the images
for n in range(1, 21): # iterate over each image
    imgName = 'Images/'+ str(n) + '.png' # set the name of the image based on the image number
    origImage = cv2.imread(imgName) # read each image as original image
    grayOrigImage = rgb2gray(origImage) # convert each image to their grayscale

    # make pixels below the half of the image to black
    for i in range(int(grayOrigImage.shape[1]/2),grayOrigImage.shape[1]): # rows from middle row of the image till bottom
        for j in range(grayOrigImage.shape[0]): # all columns 
            grayOrigImage[i][j]=0 # set pixels to 0 (black)
            
    # Computing Otsu's thresholding value
    threshold = filters.threshold_otsu(grayOrigImage)
    threshold*=2.75 # 2.75 is a multiplier to be applied for the image's thresholding


    # Applying global binary thresholding
    binarized_gray = (grayOrigImage > threshold) * 1 # set pixels more than threshold to 1
    binarized_gray*=255 # convert pixel values to 0-255 range
    cv2.imwrite(f'seg'+ str(n) + '.png', binarized_gray) # save estimated image to the same folder

    # Compute Dice Loss based on the dice formula
    estimate = binarized_gray # estimated image
    imgName = 'Pleura/' + str(n) + '.png' # set imgName to the ground truth image
    groundTruth = cv2.imread(imgName) # read ground truth image
    gray_groundTruth = rgb2gray(groundTruth) # convert ground truth image to grayscale
    target = gray_groundTruth.astype(int) # convert float values to int
    numerator = 2*target*estimate + 1 # applying DICE Loss formula
    denominator = target + estimate + 1 # applying DICE Loss formula
    frac = numerator/denominator # applying DICE Loss formula
    DICELossPartial = 1 - frac # applying DICE Loss formula
    DICELoss = np.average(DICELossPartial) # taking the average of all DICE Loss for the image as the final DICE loss

    # Display all the images - original image, ground truth image, estimated image
    plt.subplot(1, 3, 1) # set plots of size 1 row and 3 columns
    plt.title('a) Input Image') # title of first image
    plt.imshow(origImage) # show original input image first
    plt.subplot(1, 3, 2) # second subplot
    plt.title('b) Ground Truth') # set title of second image
    plt.imshow(groundTruth) # show ground truth image
    plt.subplot(1, 3, 3) # set third subplot
    plt.title('c) Estimate') # set title of third image
    plt.imshow(binarized_gray, cmap="gray") # show in grayscale
    plt.show() # show plot
    print("Dice Loss for image ",n,"=",str(abs(round(DICELoss, 5)))) # print DICE Loss for each image
    print("Otsu's threshold: ",str(abs(round(threshold, 2)))) # print Otsu's threshold found for each image
    DICELossDict[n]=

