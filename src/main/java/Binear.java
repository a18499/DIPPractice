import javax.imageio.ImageIO;
import java.awt.color.ColorSpace;
import java.awt.image.BufferedImage;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;

public class Binear {

    public Integer[] readPicture(String filepath){
        ArrayList<Integer> Allpixels = new ArrayList<>();
        Integer [] result={};
        int [] RGBArray={};

        if(Files.exists(Paths.get(filepath))){
            try {

                BufferedImage imagedata = ImageIO.read(new File(filepath));


                //Check Color space
                ColorSpace colorSpace = imagedata.getColorModel().getColorSpace();
                if ( true ) {
                    System.out.printf( "color space type: %d, is RGB = %s%n", colorSpace.getType(), colorSpace.isCS_sRGB() );
                    for ( int i = 0; i < colorSpace.getNumComponents(); i++ ) {
                        System.out.printf( "component %d = %s%n", i, colorSpace.getName( i ) );
                    }
                }

                if ( colorSpace.getType() == ColorSpace.TYPE_RGB ) {
                    System.out.printf("Bingo ");
                    Raster image = imagedata.getData();
                    //Start to assign  value
                    int numBands = image.getNumBands();
                    int height = image.getHeight();
                    int width = image.getWidth();
                    int[] pixelRow = new int[width * numBands];
                    RGBArray = new int[width*height];
                    imagedata.getRGB(0,0,170,254,RGBArray,0,width);
                    System.out.println("RGBArray "+RGBArray.length);
                    System.out.println("numBands "+numBands);
                    System.out.println("height "+height);
                    System.out.println("X "+image.getMinX());
                    System.out.println("Y "+image.getMinY());
                    for (int i = 0; i < height; i++) {
                        image.getPixels(image.getMinX(), image.getMinY() + i, width, 1, pixelRow);
                       // System.out.println("Pixels "+pixelRow.length);

                        for(int eachpixel=0;eachpixel<pixelRow.length;eachpixel++){
                            Allpixels.add(eachpixel);
                        }
                    }
                    System.out.println("AllPixels "+Allpixels.size());
                    result = Allpixels.toArray(new Integer[Allpixels.size()]);
                    System.out.println("result "+result.length);
                    //image.getPixels(image.getMinX(),image.getMinY(),image.getHeight(),image.getWidth(),pixelRow);
                    // guaranteed RGB(A) order, proceed with raster.getPixels()
                } else {
                    // well, find a way to convert from that space into RGB
                    throw new AssertionError( "color space type = " + colorSpace.getType() );
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }


        return result;
    }

    public int[] resizeBilinearGray(int[] pixels, int w, int h, int w2, int h2) {
        int[] temp = new int[w2*h2] ;
        int A, B, C, D, x, y, index, gray ;
        float x_ratio = ((float)(w-1))/w2 ;
        float y_ratio = ((float)(h-1))/h2 ;
        float x_diff, y_diff, ya, yb ;
        int offset = 0 ;
        for (int i=0;i<h2;i++) {
            for (int j=0;j<w2;j++) {
                x = (int)(x_ratio * j) ;
                y = (int)(y_ratio * i) ;
                x_diff = (x_ratio * j) - x ;
                y_diff = (y_ratio * i) - y ;
                index = y*w+x ;

                // range is 0 to 255 thus bitwise AND with 0xff
                A = pixels[index] & 0xff ;
                B = pixels[index+1] & 0xff ;
                C = pixels[index+w] & 0xff ;
                D = pixels[index+w+1] & 0xff ;

                // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
                gray = (int)(
                        A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                                C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)
                ) ;

                temp[offset++] = gray ;
            }
        }
        return temp ;
    }
    public  BufferedImage getImageFromArray(int[] pixels, int width, int height) {
        BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        WritableRaster raster = (WritableRaster) image.getData();
        raster.setPixels(0,0,width,height,pixels);
        return image;
    }
    public static void main(String [] Args){
        int [] picturePixels;
        int [] afterScaling;
        Binear test = new Binear();
        picturePixels = Arrays.stream(test.readPicture("C:\\Users\\a1849\\Desktop\\170px-Kobe_Bryant_8.jpg")).mapToInt(Integer::intValue).toArray();
        System.out.println("picturePixels"+picturePixels.length);
       // afterScaling = test.resizeBilinearGray(picturePixels,254,170,300,250);
        try {

            //Files.createFile(Paths.get("test.jpg"));
            File file = new File("test.jpg");
            BufferedImage output = test.getImageFromArray(picturePixels,170,254);
            ImageIO.write(output, "jpg", file);
           // BufferedImage output = new BufferedImage();
            //ImageIO.w
            //Write data to File


            /*
            * try {
            ImageIO.write(startImage, "jpg", file);
        } catch (IOException e) {
            e.printStackTrace();
        }
            * */
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
