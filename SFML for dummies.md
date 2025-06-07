# Chess
 
Empty Project - Visual Studio
Copiar lib, include e bin para uma pasta do projeto
Criar main.cpp nos Source Files
Propriedades - All Configurations e All Platforms
-C/C++\General\Additional Include  Directories - Adicionar o caminho para a pasta include
-Linker\General\Additional Library Directories - Adicionar o caminho para a pasta lib
Propriedades - Debug e All Platforms
Linker\Input\Additional Dependencies
-sfml-graphics-d.lib
-sfml-window-d.lib
-sfml-system-d.lib
-sfml-audio-d.lib
-sfml-network-d.lib
Propriedades - Release e All Platforms
Linker\Input\Additional Dependencies
-sfml-graphics.lib
-sfml-window.lib
-sfml-system.lib
-sfml-audio.lib
-sfml-network.lib
Properties da solução All Configurations e All Platforms - Configuration Properties\General\C++ Language Standard -> mais Atual
Copiar libs para a pasta de Debug e Release do projeto