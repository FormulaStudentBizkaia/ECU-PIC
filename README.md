# Programa del PIC de la ECU

## Descripción

"""
Este programa se encarga de escuchar la linea de CAN de los inversores (1M) para que la ECU pueda levantar
"""

## Comando para crear claves SSH

1) Inciar agente SSH
``` bash
eval "$(ssh-agent -s)"
```

2) Moverse al directorio correctopara crear la clave
``` bash
cd ~/.ssh
```

3) Crear clave ssh
``` bash
ssh-keygen
```

4) Introducir nombre de la clave
"""
Se crearan dos claves, una publica y una privada.
Ej: 
miclave (privada)
miClave.pub (pública)
"""

4) Añadir la clave privada como identificador del dispositivo
``` bash
ssh-add ~/.ssh/clavePrivada
```

5) Imprimir la clave pública
``` bash
cat ~/.ssh/clavePublica.pub
```

6) Copiar calve y meter en el usuario de gitHub
"""
Perfil >> SSH and GPG Keys >> New SSH key >> Add SSH key
"""

## Comandos para Git

7) Identificarte con nombre de usuario y email

``` bash
git config --global user.name "Tu Nombre de Git"
```

``` bash
git config --global user.email "tu@email.com de Git"
```

8) Clonar un repositorio 
``` bash
git clone url_SSH
```

9) Cambiarse de rama
``` bash
git checkout main
```

10) Entrar en nuestro proyecto del repositorio
``` bash
cd myproject
```

11) Añadir cambios

"""
El punto indica los todos los archivos dentro de la carpeta en la que estas, se puede sustituir por un unico archivo también
"""
``` bash
git add .
```

12) Hacer commit
``` bash
git commit -m "Bug arreglado"
```

13) Hacer pull a una rama
``` bash
git pull origin main
```

14) Hacer push a una rama
``` bash
git push origin main
```

## Posibles errores

"""
Si no te deja acceder al proyecto de gitHub (git clone, git pull...) vuelve a ejecutar los siguiente comandos
"""

``` bash
eval "$(ssh-agent -s)"
```

``` bash
ssh-add ~/.ssh/clavePrivada
```
