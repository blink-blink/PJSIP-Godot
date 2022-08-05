# PJSIP-Godot
Integration of PJSIP libraries in Godot via GDNative.

## Prerequisites
- PJSIP, https://www.pjsip.org/
- pjsipstreamer, https://github.com/isabyr/pjsipstreamer
- GDNative prerequisites, https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/what_is_gdnative.html

## How to build
Build using `scons` and speciify the proper parameters. An example would be `scons platform=windows target=r`
- If you're building for Windows, use `scons platform=windows` and specify target using `target=d` or `target=r`
- If building for linux, use 'scons platform=x11` or `scons platform=linux` and specify target.

## Setting up the GDNative node
This section assume you have the basic knowledge of Godot nodes. 
1. Create a Node

![image](https://user-images.githubusercontent.com/54440096/182978807-9d271ef8-df5c-4cd3-9415-81c5684850ea.png)

2. Create a new GDNativeLibrary resource.

![image](https://user-images.githubusercontent.com/54440096/182978450-812afd98-90db-4604-8929-1aa785b365ea.png)

3. Create a NativeScript resource

![image](https://user-images.githubusercontent.com/54440096/182978486-3c33226d-4d64-45dc-9f0e-664f05e2d3ca.png)

4. Specify the libraries for the GDNativeLibrary resource.

![image](https://user-images.githubusercontent.com/54440096/182978550-a244b989-64c5-48cd-a913-fcff08b34a2b.png)

4. Link the GDNativeLibrary to the NativeScript resource

![image](https://user-images.githubusercontent.com/54440096/182978715-38173546-6907-4bff-bb3f-a5189401c58e.png)

5. Attach the NativeScript resource to the created node. ![image](https://user-images.githubusercontent.com/54440096/182978883-1dcdc907-dd59-48ce-8150-e80a904397b5.png)

A demo project is included in this repo which does a local call. You can modify parameters and run the project. 
