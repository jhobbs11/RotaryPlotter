PImage testImage;
PImage returnImage;
int rows;
int columns;


void setup() {
  //Setting Size of Display - Not necessary for getting code, just for creating output field while testing.
  size(200,800);
  //Load image, Plus resize to 200px Width. The 200px dimension is arbitraty, based on pixel density for the actual printer.
  testImage = loadImage("Test Image_JPEG_Unknown Dimension.jpg");
  testImage.resize(200,0);
  testImage.loadPixels();
  rows = testImage.height;
  columns = testImage.width;
  //println(rows);
  //println(columns);
  //println(rowNum(220));
  pixels = surroundingPixels(testImage, 500, 1);
  //println(surroundingPixels(testImage,500,1));
  println(pixels);
  println(aveSurroundingPixels(pixels));
  returnImage = processImage(testImage,160);  
}

void draw() {
  image(returnImage,0,0);
}

//Function to draw new image, based on sampling from testImage. Int threshold, in 0-255, sets the cutoff value for painting black.
PImage processImage(PImage img, int threshold) {
  PImage newImage = createImage(img.width, img.height, RGB);
  newImage.loadPixels();
  for(int x=0;x<img.width*img.height;x++){
    newImage.pixels[x] = color(255,255,255);
  }
  img.updatepixels();
  for(int x=0;x<img.width*img.height;){
    //aveDensity determines how far the printer will move before analyzing the next pixel.
    float aveDensity = aveSurroundingPixels(surroundingPixels(img,x,2));
    int percWhite = (255-int(aveDensity))/255;
    int dist = round(percWhite*5);
    //Setting Pixels
    float pixelDensity = round((red(img.pixels[x])+green(img.pixels[x])+blue(img.pixels[x]))/3);
    if (pixelDensity<threshold){
      newImage.pixels[x] = color(0,0,0);
      x = x + dist;
    }
    else {
      newImage.pixels[x] = color(255,255,255);
      x = x + dist;
    }  
  }
  img.updatePixels();  
  return newImage;
}
  


//Function Calculates the average greyscale value for the surroundingPixels array. Returns a simple float, which is the r, b, and g value.
float aveSurroundingPixels(int[] array) {
  float total = 0;
  float nums = 0;
  for (int x = 0; x < array.length;x++) {
    total = total + red(array[x]);
    nums++;
  }
  return(round(total/nums));
}

//Function that takes an input p, for a pixel index from image img, and n, for distance from the pixel, and returns an array size 2n+1 x 2n+1 containing the values of pixels surrounding pixel[p], including pixel[p].  When pixel is on the edge of the image, the out-of-bounds pixels will be returned as null.
int[] surroundingPixels(PImage img,int p,int n) {
  int pixelRow = rowNum(p);
  println("pixelRow: "+pixelRow);
  int pixelCol = colNum(p);
  println("pixelCol: "+pixelCol);
  color[] surroundingPixels = new color[((2*n)+1)*((2*n)+1)];
  //int[] surroundingPixels = {};
  println(surroundingPixels);
  int arraySize = (2*n)+1;
  for (int y = 1; y <= arraySize; y++) {
    println("y = "+y);
    for (int x = 1; x <= arraySize; x++) {
      int tempPixelRow = pixelRow - n - 1 + y;
      println("tempPixelRow: "+tempPixelRow);
      int tempPixelCol = pixelCol - n - 1 + x;
      println("tempPixelCol: "+tempPixelCol);
      if (tempPixelRow < 1 | tempPixelCol < 1) {
        surroundingPixels[((y-1)*arraySize)+(x-1)] = color(255,255,255);
        //append(surroundingPixels, 0);
        println("NULL");
      }
      else {
        //convert the integer value to a greyscale color[]
        int c=img.pixels[pixelIndex(tempPixelCol, tempPixelRow)];
        float r=red(c);
        float g=green(c);
        float b=blue(c);
        int grey=round(((r+b+g)/3));
        color value= color(grey,grey,grey);
        surroundingPixels[((y-1)*arraySize)+(x-1)] = value;
        
      }
    }
  }
  return(surroundingPixels);
}

//Function to return a row number of any pixel index, p.
int rowNum(int p) {
  p++;
  return((p-(p%columns))/columns);
}

//Function to return a column number of any pixel index, p.
int colNum(int p) {
  p++;
  return(p%rows);
}

//Function to return a pixel index, given the X, Y coordinates in format (colNum, rowNum).
int pixelIndex(int x, int y) {
  return(((y-1)*columns)+x-1);
}
