import os

def rename_shaders():
    # Obtener el directorio donde se está ejecutando el script
    directory = os.getcwd()
    print(f"Buscando shaders en: {directory}\n")

    # Mapa de extensiones viejas a nuevas
    extensions_map = {
        '.fragment': '.fs',
        '.vertex': '.vs'
    }

    count = 0
    
    # Listar todos los archivos en el directorio
    for filename in os.listdir(directory):
        base_name, extension = os.path.splitext(filename)
        
        # Si la extensión está en nuestro mapa, procedemos a renombrar
        if extension in extensions_map:
            new_extension = extensions_map[extension]
            new_filename = base_name + new_extension
            
            try:
                os.rename(filename, new_filename)
                print(f"Renombrado: {filename}  ->  {new_filename}")
                count += 1
            except Exception as e:
                print(f"Error al renombrar {filename}: {e}")

    if count == 0:
        print("No se encontraron archivos .fragment o .vertex.")
    else:
        print(f"\nProceso finalizado. Se han renombrado {count} archivos.")

if __name__ == "__main__":
    rename_shaders()