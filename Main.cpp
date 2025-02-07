#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

// using namespace cv;

int main()
{
    std::string image_path = cv::samples::findFile("meninoney.png");
//  imread serve pra ler a imagem, e o IMREAD_COLOR significar que vai receber os bytes rbg
//  mas existe outros metodos como IMREAD_UNCHANGED e IMREAD_GRAYSCALE
    cv::Mat img = cv::imread(image_path, cv::IMREAD_GRAYSCALE); // mat server para armazenamento/manipulação/conversao de imagem
							    // além de imread tb temos imwrite para escrever arquivos
							    // como converter o meninonet.png para jpg
							    // cv::imwrite("meninoney.jpg", img)
    if(img.empty())
    {
        std::cout << "Could not read the image" << image_path << std::endl;
        return 1;
    }

    // imshow cria uma janela padrão sem precisar chamar namedWindows, mas se quiser ter mais controle sobre a janela
    // é importante chamar named windows pra alterar propriedades da janela.
    cv::namedWindow("Display Image", cv::WINDOW_NORMAL );
    imshow("Display Image", img); 

    // tentando fazer uma imagem pixelizada
    int blockSize = 20;
    cv::Mat smallImg, pixelizedImg;

    resize(img, smallImg, cv::Size(img.cols / blockSize, img.rows / blockSize), 0, 0, cv::INTER_LINEAR);
    resize(smallImg, pixelizedImg, cv::Size(img.cols, img.rows), 0, 0, cv::INTER_NEAREST);
    
    cv::namedWindow("Pixelized Image", cv::WINDOW_NORMAL);
    imshow("Pixelized Image", pixelizedImg);
    
    // niveis de intensidade do grayscale @#%8&WM0QO*+o=:-. 

    // int width = img.cols;
    // int height = img.rows;

    // redimensionar imagem sem perder a proporção

    double proportion = static_cast<double>(img.rows) / static_cast<double>(img.cols);
    // width - largura -------- Height - altura
    int newWidth = 320;
    int newHeight = static_cast<int>(newWidth * proportion);   
    
    cv::Mat resizedImage;
    cv::resize(pixelizedImg, resizedImage, cv::Size(newWidth, newHeight));

    cv::namedWindow("Resized Image", cv::WINDOW_NORMAL);
    imshow("Resized Image", resizedImage);

    cv::waitKey(0); // espera que uma tecla seja precionada, se não o programa fecha muito rapido

    return 0;
}
