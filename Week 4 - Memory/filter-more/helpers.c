#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculation of average value of RGB
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Assigning average value to R/G & B
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // j = width / 2.0 as we want to swap pixels, so half is enough. otherwise it will swap again a build the image as original
        for (int j = 0; j < width / 2.0; j++)
        {
            int tmp_R = image[i][j].rgbtRed;
            int tmp_G = image[i][j].rgbtGreen;
            int tmp_B = image[i][j].rgbtBlue;

            // width - j - 1 is because start from last pixel to swap with first one from same row. -1 because we start from 0 and -j because we increment towards center of image
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            image[i][width - j - 1].rgbtRed = tmp_R;
            image[i][width - j - 1].rgbtGreen = tmp_G;
            image[i][width - j - 1].rgbtBlue = tmp_B;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy of image creation
    RGBTRIPLE image_bis[height][width];

    // This has to be done otherwise modify pixels will have an influence on other pixels modification and thus get wrong results

    // loop that copy image to image_bis
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_bis[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialization of sum variables for each color
            int sum_R = 0;
            int sum_G = 0;
            int sum_B = 0;
            // initialization of pixel_count
            int pixel_count = 0;

            // MIDDLE PIXELS
            if (i > 0 && i < height - 1 && j > 0 && j < width - 1)
            {
                // loop to calculate blur for middle pixels - 3x3 pixels
                for (int k = i - 1; k <= i + 1; k++)
                {
                    for (int l = j - 1; l <= j + 1; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // TOP EDGE
            if (i == 0 && j > 0 && j < width - 1)
            {
                // here we are avoiding k = i-1 as it exceed image
                for (int k = i; k <= i + 1; k++)
                {
                    for (int l = j - 1; l <= j + 1; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // BOTTOM EDGE
            if (i == height - 1 && j > 0 && j < width - 1)
            {
                // here we are avoiding k = i+1 as it exceed image
                for (int k = i - 1; k <= i; k++)
                {
                    for (int l = j - 1; l <= j + 1; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // LEFT EDGE
            if (i > 0 && i < height - 1 && j == 0)
            {
                // here we are avoiding l = j-1 as it exceed image
                for (int k = i - 1; k <= i + 1; k++)
                {
                    for (int l = j; l <= j + 1; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // RIGHT EDGE
            if (i > 0 && i < height - 1 && j == width - 1)
            {
                // here we are avoiding l = j+1 as it exceed image
                for (int k = i - 1; k <= i + 1; k++)
                {
                    for (int l = j - 1; l <= j; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // TOP LEFT CORNER
            if (i == 0 && j == 0)
            {
                // here we are avoiding k = i-1 & l = j-1 as it exceed image
                for (int k = i; k <= i + 1; k++)
                {
                    for (int l = j; l <= j + 1; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // TOP RIGHT CORNER
            if (i == 0 && j == width - 1)
            {
                // here we are avoiding k = i-1 & l = j+1 as it exceed image
                for (int k = i; k <= i + 1; k++)
                {
                    for (int l = j - 1; l <= j; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // BOTTOM RIGHT CORNER
            if (i == height - 1 && j == width - 1)
            {
                // here we are avoiding k = i+1 & l = j+1 as it exceed image
                for (int k = i - 1; k <= i; k++)
                {
                    for (int l = j - 1; l <= j; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }

            // BOTTOM LEFT CORNER
            if (i == height - 1 && j == 0)
            {
                // here we are avoiding k = i+1 & l = j-1 as it exceed image
                for (int k = i - 1; k <= i; k++)
                {
                    for (int l = j; l <= j + 1; l++)
                    {
                        sum_R = sum_R + image_bis[k][l].rgbtRed;
                        sum_G = sum_G + image_bis[k][l].rgbtGreen;
                        sum_B = sum_B + image_bis[k][l].rgbtBlue;

                        pixel_count = pixel_count + 1;
                    }
                }
            }
            image[i][j].rgbtRed = round(sum_R / (pixel_count * 1.0));
            image[i][j].rgbtGreen = round(sum_G / (pixel_count * 1.0));
            image[i][j].rgbtBlue = round(sum_B / (pixel_count * 1.0));
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // creation of Gx and Gy
    int Gx[3][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int Gy[3][3] = {{-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // copy of image creation
    RGBTRIPLE image_bis[height][width];

    // loop that copy image to image_bis
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_bis[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialization of Gx and Gy for each color
            int Gx_R = 0;
            int Gx_G = 0;
            int Gx_B = 0;
            int Gy_R = 0;
            int Gy_G = 0;
            int Gy_B = 0;

            // MIDDLE PIXELS
            if (i > 0 && i < height - 1 && j > 0 && j < width - 1)
            {
                // here k & l goes from -1 to 1 to loop through all pixel around central pixel
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // Gx
                        Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                        Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                        Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        // k+1 & l+1 because Gx & Gy must loop from 0 to 2 as it's a 3x3 matrix
                        // i+k & j+l because we want to loop through all pixels next to the central pixel

                        // Gy
                        Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                        Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                        Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                    }
                }
            }

            // TOP EDGE
            if (i == 0 && j > 0 && j < width - 1)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (i + k >= 0)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // BOTTOM EDGE
            if (i == height - 1 && j > 0 && j < width - 1)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (i + k < height)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // LEFT EDGE
            if (i > 0 && i < height - 1 && j == 0)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (j + l >= 0)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // RIGHT EDGE
            if (i > 0 && i < height - 1 && j == width - 1)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (j + l < width)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // TOP LEFT CORNER
            if (i == 0 && j == 0)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (i + k >= 0 && j + l >= 0)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // TOP RIGHT CORNER
            if (i == 0 && j == width - 1)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (i + k >= 0 && j + l < width)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // BOTTOM RIGHT CORNER
            if (i == height - 1 && j == width - 1)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (i + k < height && j + l < width)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // BOTTOM LEFT CORNER
            if (i == height - 1 && j == 0)
            {
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // here we skip border pixels as we suppose it's a solid black border so image_bis[i + k][j + l].rgbtRed/rgbtGreen/rgbtBlue = 0
                        if (i + k < height && j + l >= 0)
                        {
                            // Gx
                            Gx_R = Gx_R + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gx_G = Gx_G + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gx_B = Gx_B + Gx[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;

                            // Gy
                            Gy_R = Gy_R + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtRed;
                            Gy_G = Gy_G + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtGreen;
                            Gy_B = Gy_B + Gy[k + 1][l + 1] * image_bis[i + k][j + l].rgbtBlue;
                        }
                    }
                }
            }

            // Sobel Calculation
            int RED = round(sqrt(Gx_R * Gx_R + Gy_R * Gy_R));
            int GREEN = round(sqrt(Gx_G * Gx_G + Gy_G * Gy_G));
            int BLUE = round(sqrt(Gx_B * Gx_B + Gy_B * Gy_B));

            // checking for values higher to 255
            if (RED > 255)
            {
                RED = 255;
            }

            if (GREEN > 255)
            {
                GREEN = 255;
            }

            if (BLUE > 255)
            {
                BLUE = 255;
            }

            // color assignment for each pixels
            image[i][j].rgbtRed = RED;
            image[i][j].rgbtGreen = GREEN;
            image[i][j].rgbtBlue = BLUE;
        }
    }
    return;
}
