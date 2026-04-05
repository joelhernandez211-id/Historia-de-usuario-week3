import csv

# INVENTARIO
inventario = []


# -------- FUNCIONES CRUD --------

def agregar_producto(nombre, precio, cantidad):
    inventario.append({
        "nombre": nombre,
        "precio": precio,
        "cantidad": cantidad
    })


def mostrar_inventario():
    if not inventario:
        print("Inventario vacío")
        return

    for p in inventario:
        print(f"{p['nombre']} - Precio: {p['precio']} - Cantidad: {p['cantidad']}")


def buscar_producto(nombre):
    for p in inventario:
        if p["nombre"].lower() == nombre.lower():
            return p
    return None


def actualizar_producto(nombre, precio=None, cantidad=None):
    producto = buscar_producto(nombre)
    if producto:
        if precio is not None:
            producto["precio"] = precio
        if cantidad is not None:
            producto["cantidad"] = cantidad
        return True
    return False


def eliminar_producto(nombre):
    producto = buscar_producto(nombre)
    if producto:
        inventario.remove(producto)
        return True
    return False


# -------- ESTADÍSTICAS --------

def calcular_estadisticas():
    if not inventario:
        return None

    total_unidades = sum(p["cantidad"] for p in inventario)
    valor_total = sum(p["precio"] * p["cantidad"] for p in inventario)

    mas_caro = max(inventario, key=lambda p: p["precio"])
    mayor_stock = max(inventario, key=lambda p: p["cantidad"])

    return total_unidades, valor_total, mas_caro, mayor_stock


# -------- ARCHIVOS --------

def guardar_csv(ruta):
    if not inventario:
        print("Inventario vacío")
        return

    try:
        with open(ruta, "w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow(["nombre", "precio", "cantidad"])

            for p in inventario:
                writer.writerow([p["nombre"], p["precio"], p["cantidad"]])

        print("Guardado correctamente")

    except:
        print("Error al guardar archivo")


def cargar_csv(ruta):
    nuevos = []
    errores = 0

    try:
        with open(ruta, "r", encoding="utf-8") as f:
            reader = csv.reader(f)
            encabezado = next(reader)

            if encabezado != ["nombre", "precio", "cantidad"]:
                print("Formato incorrecto")
                return []

            for fila in reader:
                if len(fila) != 3:
                    errores += 1
                    continue

                try:
                    nombre = fila[0]
                    precio = float(fila[1])
                    cantidad = int(fila[2])

                    if precio < 0 or cantidad < 0:
                        raise ValueError

                    nuevos.append({
                        "nombre": nombre,
                        "precio": precio,
                        "cantidad": cantidad
                    })

                except:
                    errores += 1

        print(f"Filas inválidas: {errores}")
        return nuevos

    except:
        print("Error al cargar archivo")
        return []


# -------- MENÚ --------

opcion = 0

while opcion != 9:

    print("\n--- MENÚ ---")
    print("1. Agregar")
    print("2. Mostrar")
    print("3. Buscar")
    print("4. Actualizar")
    print("5. Eliminar")
    print("6. Estadísticas")
    print("7. Guardar CSV")
    print("8. Cargar CSV")
    print("9. Salir")

    try:
        opcion = int(input("Opción: "))

        if opcion == 1:
            nombre = input("Nombre: ")
            precio = float(input("Precio: "))
            cantidad = int(input("Cantidad: "))
            agregar_producto(nombre, precio, cantidad)

        elif opcion == 2:
            mostrar_inventario()

        elif opcion == 3:
            nombre = input("Buscar: ")
            print(buscar_producto(nombre) or "No encontrado")

        elif opcion == 4:
            nombre = input("Producto: ")
            precio = input("Nuevo precio: ")
            cantidad = input("Nueva cantidad: ")

            actualizar_producto(
                nombre,
                float(precio) if precio else None,
                int(cantidad) if cantidad else None
            )

        elif opcion == 5:
            nombre = input("Eliminar: ")
            eliminar_producto(nombre)

        elif opcion == 6:
            stats = calcular_estadisticas()
            if stats:
                print("Unidades:", stats[0])
                print("Valor total:", stats[1])
                print("Más caro:", stats[2])
                print("Mayor stock:", stats[3])
            else:
                print("Inventario vacío")

        elif opcion == 7:
            ruta = input("Ruta archivo: ")
            guardar_csv(ruta)

        elif opcion == 8:
            ruta = input("Ruta archivo: ")
            nuevos = cargar_csv(ruta)

            if nuevos:
                decision = input("¿Reemplazar? S/N: ").upper()

                if decision == "S":
                    inventario = nuevos
                else:
                    for n in nuevos:
                        existente = buscar_producto(n["nombre"])
                        if existente:
                            existente["cantidad"] += n["cantidad"]
                            existente["precio"] = n["precio"]
                        else:
                            inventario.append(n)

        elif opcion == 9:
            print("Fin del programa")

        else:
            print("Opción inválida")

    except:
        print("Error en los datos")
