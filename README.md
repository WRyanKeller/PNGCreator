# PNGCreator
A project that aims to allow you to use CityEngine tutorials to create something unique to you :)

## Background / Goal
The project began as a final project for a Spatial Algorithm class which was mostly focused on ArcGIS but also spent a week looking at CityEngine. The CityEngine [tutorials 1-3](https://doc.arcgis.com/en/cityengine/latest/tutorials/tutorial-1-essential-skills.htm) showed immense potential to create entire cityscapes from just a few png maps! While there are already photo editors out there, my goal was to have a tool that would create maps specifically for this application.  

## Function
The PNGCreator as of v0.1.0 is able to generate a series of random pngs which are inspired by those provided with the CityEngine tutorial. While they aren't polished, they should work well enough. Each uses a different algorithm:
- Topology - displays landmarks generated by a custom noise algorithm
- Obstacles - displays the landmarks in black on a white background
- Area Types - Uses the landmark algorithm to generate but draws with diminishing opacity to create a fuzzy edge
- Elevation - Attempts (VERY wip) to create areas which gradiate lighter, representing hills
- Skyline - Similar to area types  
These five maps can be randomly generated using a built in menu and then saved, which will create or overwrite five pngs in the executable's folder. These can then be substituted in for the provided pngs throughout the steps of the CityEngine [tutorials 1-3](https://doc.arcgis.com/en/cityengine/latest/tutorials/tutorial-1-essential-skills.htm) :)!

## Demo
[This](https://www.youtube.com/watch?v=OE9DkvGlNN8) is a soundless demo that generated the sample output provided :)

## Download and Use
Download latest version [from here](https://github.com/WRyanKeller/PNGCreator/)

## Important Credits!
This has been made possible by:
The PNG Writer library by [Paul Blackburn](https://github.com/individual61) and [Axel Huebl](https://github.com/ax3l)  
The [OpenGL](https://www.opengl.org/) library  
Code reference from [Dr. Chao Peng](https://people.rit.edu/cxpigm/)  

## Author
Created by: Ryan Keller  
GitHub: [WRyanKeller](https://github.com/WRyanKeller/)  
Personal Site: WIP  
Contact about PNGCreator: please email any inquiries or suggestions to william.ryan.keller@gmail.com with the subject line "pngcreator"  

## Liscense
The GPL-3.0 Liscence allows free usage of any of this, so as long as the product is also open-source. Enjoy :)
