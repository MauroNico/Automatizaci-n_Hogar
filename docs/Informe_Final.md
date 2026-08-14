# Smart Home OS - Core Architecture (Domain-Driven Design)

Este proyecto implementa el núcleo de un sistema operativo para casas inteligentes en C++ moderno, cumpliendo estrictamente con los principios SOLID y Clean Code para un Trabajo Final universitario. 

La arquitectura destaca por aplicar **Diseño Guiado por el Dominio (DDD)**: las clases reflejan conceptos 100% reales de la industria del Internet de las Cosas (IoT), ocultando la terminología académica abstracta, logrando que el código sea comprensible tanto técnica como comercialmente.

## Arquitectura y Patrones de Diseño

A continuación, se detalla la correspondencia entre los conceptos del dominio implementados y los 8 patrones de diseño (GoF) utilizados estructuralmente:

1. **Singleton (`HomeManager`)**
   - **Dominio:** El Hub o Cerebro central de la casa inteligente.
   - **Resolución:** Garantiza un único punto de acceso global que administra coordinadamente los dispositivos, modos de seguridad y suscripciones a eventos.

2. **Composite (`IDevice`, `DeviceGroup`, `SmartLight`, `SmartLock`)**
   - **Dominio:** Zonas y Grupos de Hardware.
   - **Resolución:** Proporciona un manejo de jerarquías uniforme, permitiendo al sistema encender un único dispositivo o todo un "Grupo Exterior" utilizando exactamente el mismo método `turnOn()`.

3. **Adapter (`ZigbeeAPI`, `ZigbeeDeviceHandler`)**
   - **Dominio:** Driver / Integración de Hardware de terceros.
   - **Resolución:** En lugar de acoplar la lógica del sistema a protocolos específicos de fabricantes, el `ZigbeeDeviceHandler` adapta nuestros comandos amigables de alto nivel al envío de *payloads* de red de bajo nivel requeridos por las radios mesh Zigbee.

4. **Observer (`ISensorListener`, `MotionSensor`)**
   - **Dominio:** Sistema Reactivo de Eventos de Hardware.
   - **Resolución:** Al adoptar una arquitectura de pub/sub con `ISensorListener`, cuando el hardware detecta movimiento, dispara un evento que despierta al `HomeManager` de forma inmediata, eliminando ineficientes consultas constantes (polling).

5. **State (`SecuritySystem`, `ISecurityMode`, `ArmedMode`, `DisarmedMode`, `NightMode`)**
   - **Dominio:** Modos de Operación del Panel de Alarma.
   - **Resolución:** Elimina la necesidad de condicionales (if/else) complejos y anidados. El panel responde de manera polimórfica al mismo evento sensorial: encendiendo luces en el modo noche, o activando sirenas y notificando a las autoridades si está armado.

6. **Factory Method (`DeviceProvisioner`)**
   - **Dominio:** Proceso de aprovisionamiento de dispositivos en red.
   - **Resolución:** Centraliza la complejidad de inicializar el hardware. Si se provisiona un `"HighSecurityLock"`, este componente se encarga de ensamblarlo dinámicamente junto a sus características extra de seguridad (decoradores) aislando de esta lógica al cliente.

7. **Decorator (`AuditedDevice`, `AutoLockingDevice`)**
   - **Dominio:** Capas de Seguridad Adicional por Firmware (Addons).
   - **Resolución:** Permite agregar capacidades cruciales, como auditoría inmutable de accesos en una puerta (`AuditedDevice`), envolviendo dinámicamente un dispositivo estándar sin necesidad de recurrir a herencia múltiple inmanejable.

8. **Template Method (`SmartRoutine`, `MorningRoutine`, `NightRoutine`)**
   - **Dominio:** Macros de Automatización del Hogar.
   - **Resolución:** Establece el algoritmo core o receta general de las rutinas (`run()`), forzando y orquestando a cada automatización concreta a ajustar el ecosistema de la casa interactuando obligatoriamente tanto con los grupos de luces como con el panel de alarma.

## Consideraciones particulares de diseño

### Composite Transparente vs. Seguro
En la implementación del patrón **Composite** (`IDevice`, `DeviceGroup`, etc.), se optó por la variante "transparente" clásica de GoF. Esto significa que los métodos `add()` y `remove()` se definen en la interfaz base `IDevice` con una implementación vacía por defecto, en lugar de ser exclusivos de `DeviceGroup`.

- **Trade-off (Ventaja - Simplicidad):** Se prioriza la uniformidad. El cliente puede tratar a todos los objetos (nodos hoja o compuestos) exactamente de la misma manera a través de la interfaz `IDevice`, sin necesidad de hacer *downcasting* (ej. `dynamic_cast`) ni preguntar de qué tipo son.
- **Trade-off (Desventaja - Seguridad):** Se sacrifica seguridad en tiempo de compilación. Técnicamente, el código permite llamar a `add()` sobre un `SmartLight` (un nodo hoja que no debería tener hijos). Al ser "transparente", la llamada compila y simplemente no hace nada (falla silenciosamente), lo cual exige mayor cuidado por parte del programador cliente.

## Compilación y Ejecución (CMake)

El proyecto separa de forma estricta las interfaces/definiciones (`.h` en `include/`) de sus implementaciones (`.cpp` en `src/`).

```bash
mingw
```
