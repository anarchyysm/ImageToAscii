
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

    cv::waitKey(0); // espera que uma tecla seja precionada, se não o programa fecha muito rapido

    return 0;
}
