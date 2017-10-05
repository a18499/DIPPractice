import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.awt.image.Raster;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Binear {

    public int[] readPicture(String filepath){
        int [] result={};
        int [] results = new int[170*3*254];
        if(Files.exists(Paths.get(filepath))){
            try {
                byte [] data = Files.readAllBytes(Paths.get(filepath));
                BufferedImage imagedata = ImageIO.read(new File(filepath));

                System.out.println("Height "+imagedata.getHeight());
                System.out.println("Width "+imagedata.getWidth());
                Raster image = imagedata.getData();
                result = image.getPixels(1,1,170,254,results);
                System.out.println("result size  "+result.length);
                for(int i=0 ;i<result.length;i++){
                    System.out.println("Data "+result[i]);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


        return result;
    }


    public static void main(String [] Args){
        Binear test = new Binear();
        test.readPicture("C:\\Users\\a1849\\Desktop\\170px-Kobe_Bryant_8.jpg");

    }
}
