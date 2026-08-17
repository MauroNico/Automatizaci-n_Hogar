# Smart Home OS - Core Architecture (Domain-Driven Design)

Este proyecto implementa el núcleo de un sistema operativo para casas inteligentes en C++ moderno, cumpliendo estrictamente con los principios SOLID y Clean Code para un Trabajo Final universitario. 

La arquitectura destaca por aplicar **Diseño Guiado por el Dominio (DDD)**: las clases reflejan conceptos 100% reales de la industria del Internet de las Cosas (IoT), ocultando la terminología académica abstracta, logrando que el código sea comprensible tanto técnica como comercialmente.

## Compilación y Ejecución

El proyecto separa de forma estricta las interfaces/definiciones (`.h` en `include/`) de sus implementaciones (`.cpp` en `src/`).

Para compilar y ejecutar el sistema interactivo desde consola (en Windows):
```bash
mingw32-make
.\SmartHomeOS.exe
```
*(Para limpiar los archivos de compilación, utiliza `mingw32-make clean`)*

## Arquitectura y Patrones de Diseño

A continuación, se detalla la correspondencia entre los conceptos del dominio implementados y los 8 patrones de diseño (GoF) utilizados estructuralmente. **Todos estos patrones pueden ser probados en vivo a través de la interfaz de consola interactiva.**

1. **Singleton (`HomeManager`)**
   - **Dominio:** El Hub o Cerebro central de la casa inteligente.
   - **En la CLI:** Al iniciar el sistema, todos los menús interactúan con la misma instancia global que mantiene el registro de tus dispositivos y estados.

2. **Composite (`IDevice`, `DeviceGroup`, `SmartLight`, `SmartLock`)**
   - **Dominio:** Zonas y Grupos de Hardware.
   - **En la CLI:** El menú "Gestionar dispositivos" permite crear un Grupo (opción 8) y agregarle dispositivos existentes (opción 9). Luego puedes encender o apagar el grupo entero ingresando su ID, y el comando se propagará a todos sus hijos transparentemente.

3. **Adapter (`ZigbeeAPI`, `ZigbeeDeviceHandler`)**
   - **Dominio:** Driver / Integración de Hardware de terceros.
   - **En la CLI:** Al crear un "Zigbee Relay" (opción 4), el sistema utiliza el adaptador de forma transparente. Cuando lo enciendes, verás en consola los logs de red de bajo nivel traduciendo el comando.

4. **Observer (`ISensorListener`, `MotionSensor`)**
   - **Dominio:** Sistema Reactivo de Eventos de Hardware.
   - **En la CLI:** El menú "Simular sensores" dispara un evento que despierta al `HomeManager` y al `SecuritySystem` de forma inmediata, procesando la intrusión sin necesidad de hacer *polling* (consultas constantes).

5. **State (`SecuritySystem`, `ISecurityMode`, `ArmedMode`, `DisarmedMode`, `NightMode`)**
   - **Dominio:** Modos de Operación del Panel de Alarma.
   - **En la CLI:** El menú "Gestionar seguridad" permite cambiar el estado de la alarma en tiempo de ejecución. Al simular un sensor, la reacción cambiará drásticamente dependiendo del estado actual (sirenas vs. luces vs. ignorar).

6. **Factory Method (`DeviceProvisioner`)**
   - **Dominio:** Proceso de aprovisionamiento de dispositivos en red.
   - **En la CLI:** Las opciones de creación de dispositivos (1 a 4) utilizan las *Factories* concretas por detrás para instanciar dinámicamente la clase derivada correspondiente sin acoplar directamente el `main`.

7. **Decorator (`AuditedDevice`, `AutoLockingDevice`)**
   - **Dominio:** Capas de Seguridad Adicional por Firmware (Addons).
   - **En la CLI:** La opción "Envolver dispositivo en Auto-Locking" (opción 7) permite seleccionar un dispositivo existente y añadirle la capacidad de auto-bloqueo dinámicamente en memoria, actualizando el puntero en el Hub.

8. **Template Method (`SmartRoutine`, `MorningRoutine`, `NightRoutine`)**
   - **Dominio:** Macros de Automatización del Hogar.
   - **En la CLI:** El menú "Gestionar rutinas" permite disparar las rutinas preprogramadas, las cuales siguen un esqueleto estricto estipulado por la superclase abstracta pero con implementaciones concretas distintas para la mañana o la noche.

---

## Guía de Usuario - CLI Interactiva

Al ejecutar `SmartHomeOS.exe`, serás recibido por el menú principal:

```text
========================================
SMART HOME OS
=============
1. Gestionar dispositivos
2. Gestionar seguridad
3. Gestionar rutinas
4. Simular sensores
5. Estado del sistema
6. Salir
```

### 1. Gestión de Dispositivos (Factory, Composite y Decorator)
En este submenú podrás crear luces, cerraduras y relés ingresando un `ID` (sin espacios, ej. `luz1`) y un `Nombre` descriptivo. 
- Puedes agrupar dispositivos creando un "Grupo" (opción 8) y añadiendo dispositivos individuales (opción 9). Luego, puedes encender/apagar el ID del grupo para afectar a todos en bloque.
- Puedes utilizar la opción "Envolver dispositivo" (opción 7) para inyectar dinámicamente un Decorador (`AutoLockingDevice`) a una cerradura o luz existente.

### 2. Gestión de Seguridad (State)
Permite alternar el modo del Panel de Alarma entre `Armed`, `NightMode` y `Disarmed`. 
- **Tip para la demostración:** Cambia el modo a `Armed` y luego usa la opción "Simular evento de sensor" para ver cómo saltan las sirenas. Luego, cámbialo a `NightMode` y vuelve a simular el evento; notarás que la reacción cambia por completo, demostrando el polimorfismo del patrón State en vivo.

### 3. Gestión de Rutinas (Template Method)
Permite ejecutar `MorningRoutine` o `NightRoutine`. Observa cómo cada rutina interactúa de manera predecible pero distinta con las luces y el panel de seguridad.
*(Nota: Por diseño arquitectónico estricto de Template Method, la creación de nuevas rutinas requiere herencia C++, por lo que no pueden armarse al vuelo desde la consola, delegando esa tarea al tiempo de desarrollo).*

### 4. Simulación de Sensores (Observer)
Emite un pulso virtual desde un `MotionSensor`. Si el Hub está a la escucha (que lo está desde el inicio del programa), interceptará el evento de inmediato y tomará decisiones en conjunción con el Panel de Seguridad.

### 5. Estado del Sistema
Muestra indicadores en vivo extraídos de los *getters* del dominio, permitiéndote comprobar cuántos dispositivos has instanciado en tu sesión dinámica y en qué modo se encuentra la alarma.

---

## Guía Técnica — Crear nuevas rutinas (Para Desarrolladores)

Las rutinas del sistema utilizan el patrón **Template Method**.
La clase base abstracta `SmartRoutine` define la estructura o *esqueleto* general de ejecución de una rutina (`run()`), mientras que las clases concretas definen las acciones específicas en tiempo de compilación.

Para agregar una nueva rutina como desarrollador:

1. **Crear la clase:** Debe heredar de `SmartRoutine` (ej. `class LeavingHomeRoutine : public SmartRoutine`).
2. **Implementar los pasos abstractos:** Sobrescribir los métodos protegidos `configureLighting()` y `configureSecurity()`.
3. **Instanciar y ejecutar:** Registrarla en los menús de `main.cpp` instanciándola y llamando al método público `run()`.

```cpp
// Ejemplo conceptual de implementación:
void LeavingHomeRoutine::configureLighting() {
    hub->turnOffAll();
}

void LeavingHomeRoutine::configureSecurity() {
    hub->getSecuritySystem()->setArmedMode();
    hub->lockAllDoors();
}
```

Este esquema garantiza que el sistema respete el principio **Open/Closed** (Abierto a la extensión, cerrado a la modificación), permitiendo a los desarrolladores añadir rutinas infinitas sin jamás tocar la lógica central de la clase `SmartRoutine`.