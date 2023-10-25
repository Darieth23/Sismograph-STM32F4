# Solución Laboratorio 4
Esta es la solución de Laboratorio 4 del curso Laboratorio de Microcontroladores, II-2023-UCR.

# Compilación
Para compilar este ejemplo es necesario tener previamente descargada y compilada
la biblioteca de opencm3. Se debe editar el Makefile para indicar el directorio
donde se encuentra la biblioteca. El ejemplo compilado queda en el directorio build.

En una consola corrar los siguientes comandos.

Es necesario correr el comando Make en primera instancia:

make

En caso de tener solo un archivo con extension .elf se puede generar el archivo .bin con:

arm-none-eabi-objcopy -O binary Sismografo.elf  Sismografo.bin

Para quemar/subir el binario a la placa se puede utilizar el programa st-flash, por ejemplo:

st-flash --reset write Sismografo.bin 0x8000000

