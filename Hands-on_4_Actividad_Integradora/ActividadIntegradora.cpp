#include "raylib.h"
#include <cmath>

int main()
{
    // Tamaño de la ventana
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Laboratorio Espacial Avanzado - Paimon y Lumine");
    SetTargetFPS(60);

    // Configuracion de la camara 3d
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 15.0f, 25.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // CARGA DE MODELOS 3D
    Model modeloPaimon = LoadModel("paimon.glb");
    float escalaPaimon = 0.6f;

    Model modeloLumine = LoadModel("genshin_impact_lumine.glb");
    float escalaLumine = 0.4f;

    // VARIABLES ENTORNO 2D (Lado Izquierdo)
    Vector2 posRebote2D = { 150.0f, 210.0f };
    float rapidez2D = 250.0f;
    int dir2D = 1;
    float anguloOrbita2D = 0.0f;
    float tiempoSeno2D = 0.0f;

    // VARIABLES ENTORNO 3D (Lado Derecho)
    // 1. Rotacion (extrema derecha)
    float anguloRotacion3D = 0.0f;
    Vector3 posRotacion3D = { 7.0f, 2.0f, 0.0f };

    // 2. Rebote vertical (extrema izquierda)
    Vector3 posRebote3D = { -7.0f, 2.0f, 0.0f };
    float rapidez3D = 5.0f;
    int dir3D = 1;

    // 3. Movimiento senoidal (centro del escenario)
    Vector3 posSeno3D = { 0.0f, 2.0f, 0.0f };
    float tiempoSeno3D = 0.0f;

    // 4. Orbita circular (gira alrededor de lumine senoidal)
    float anguloOrbita3D = 0.0f;

    // 5. Trayectoria parabolica (fondo del escenario)
    Vector3 posTrayectoria3D = { -5.0f, 0.5f, -8.0f };
    float tiempoTrayectoria3D = 0.0f;
    const float gravedad = 9.8f;
    const float velInicialY = 10.0f;
    const float velInicialZ = 4.0f;

    Vector3 sinRotacion = { 0.0f, 0.0f, 0.0f };
    Vector3 ejeRotacionY = { 0.0f, 1.0f, 0.0f };

    // Creacion del lienzo de textura para contener el renderizado 3D de forma aislada
    RenderTexture2D target3D = LoadRenderTexture(800, 900);

    // Bucle principal
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Logica en 2d
        posRebote2D.x += (rapidez2D * dir2D) * dt;
        if (posRebote2D.x > 700 || posRebote2D.x < 100) dir2D *= -1;

        anguloOrbita2D += 2.0f * dt;
        tiempoSeno2D += dt;

        // Logica en 3d
        // Rotacion de los modelos
        anguloRotacion3D += 60.0f * dt;

        // Rebote de lumine en y
        posRebote3D.y += (rapidez3D * dir3D) * dt;
        if (posRebote3D.y > 7.0f) { posRebote3D.y = 7.0f; dir3D = -1; }
        if (posRebote3D.y < 0.5f) { posRebote3D.y = 0.5f; dir3D = 1; }

        // Seno de lumine en z
        tiempoSeno3D += dt;
        posSeno3D.z = sinf(tiempoSeno3D * 2.5f) * 4.5f;

        // Orbita de paimon alrededor de lumine senoidal
        anguloOrbita3D += 1.2f * dt;
        float orbX_3D = posSeno3D.x + cosf(anguloOrbita3D) * 3.5f;
        float orbZ_3D = posSeno3D.z + sinf(anguloOrbita3D) * 3.5f;

        // Tiro parabolico de paimon
        tiempoTrayectoria3D += dt;
        posTrayectoria3D.z = -8.0f + velInicialZ * tiempoTrayectoria3D;
        posTrayectoria3D.y = 0.5f + (velInicialY * tiempoTrayectoria3D - 0.5f * gravedad * tiempoTrayectoria3D * tiempoTrayectoria3D);

        if (posTrayectoria3D.y < 0.5f) {
            tiempoTrayectoria3D = 0.0f;
            posTrayectoria3D = (Vector3){ -5.0f, 0.5f, -8.0f };
        }

        // ENTORNO 3D (Lado Derecho - Lumine & Paimon)
        BeginTextureMode(target3D);
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawGrid(20, 1.0f);

        Vector3 escalaL = { escalaLumine, escalaLumine, escalaLumine };
        Vector3 escalaP = { escalaPaimon, escalaPaimon, escalaPaimon };

        // A. Lumine rotando (derecha)
        DrawModelEx(modeloLumine, posRotacion3D, ejeRotacionY, anguloRotacion3D, escalaL, WHITE);

        // B. Lumine rebotando (izquierda)
        DrawModelEx(modeloLumine, posRebote3D, sinRotacion, 0.0f, escalaL, WHITE);

        // C. Lumine senoidal (centro)
        DrawModelEx(modeloLumine, posSeno3D, sinRotacion, 0.0f, escalaL, WHITE);

        // D. Paimon en orbita (gira alrededor de lumine del centro)
        Vector3 escalaPaimonSat = { escalaPaimon * 0.6f, escalaPaimon * 0.6f, escalaPaimon * 0.6f };
        DrawModelEx(modeloPaimon, (Vector3) { orbX_3D, 2.0f, orbZ_3D }, ejeRotacionY, anguloRotacion3D * 1.5f, escalaPaimonSat, WHITE);

        // E. Paimon en trayectoria parabolica (fondo)
        DrawModelEx(modeloPaimon, posTrayectoria3D, sinRotacion, 0.0f, escalaP, WHITE);

        EndMode3D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Volcar el lienzo tridimensional intermedio en la mitad derecha de la ventana
        DrawTextureRec(target3D.texture, (Rectangle) { 0, 0, 800, -900 }, (Vector2) { 800, 0 }, WHITE);

        // Linea divisoria central
        DrawLine(800, 0, 800, screenHeight, LIGHTGRAY);

        // ENTORNO 2D (Lado Izquierdo)
        DrawText("ANALISIS MATEMÁTICO 2D", 40, 40, 26, DARKBLUE);

        // Bloque 1: Rebote lineal (Espacio en Y: 110 a 230)
        DrawText("Traslacion y rebote horizontal", 60, 110, 18, GRAY);
        DrawLine(100, 210, 700, 210, LIGHTGRAY);
        DrawCircleV(posRebote2D, 18, ORANGE);

        // Bloque 2: Movimiento senoidal (Espacio en Y: 290 a 490)
        DrawText("Movimiento senoidal periodico", 60, 290, 18, GRAY);
        float posY_Seno = 400.0f + sinf(tiempoSeno2D * 4.0f) * 60.0f; // Oscila bien centrado en Y = 400
        DrawCircle(400, posY_Seno, 18, SKYBLUE);

        // Bloque 3: Orbita circular (Espacio en Y: 550 a 830)
        DrawText("Trayectoria orbital en plano cartesiano", 60, 550, 18, GRAY);
        Vector2 centro2D = { 400, 710 };
        float orbX_2D = centro2D.x + cosf(anguloOrbita2D) * 80.0f;
        float orbY_2D = centro2D.y + sinf(anguloOrbita2D) * 80.0f;
        DrawCircleLines(centro2D.x, centro2D.y, 80.0f, LIGHTGRAY);
        DrawCircle(centro2D.x, centro2D.y, 12, DARKGRAY);
        DrawCircle(orbX_2D, orbY_2D, 15, PURPLE);

        // TEXTOS DE INTERFAZ
        DrawText("VISUALIZACIÓN ESPACIAL 3D", 840, 40, 26, DARKGREEN);

        DrawText("[Lumine] Izquierda: Rebote cinematico (Eje Y)", 850, 120, 16, MAROON);
        DrawText("[Lumine] Centro: Oscilacion senoidal (Eje Z)", 850, 150, 16, DARKBLUE);
        DrawText("[Lumine] Derecha: Rotacion sobre eje Y", 850, 180, 16, DARKGREEN);
        DrawText("[Paimon] Satelite: Orbita circular (Alrededor de lumine)", 850, 210, 16, PURPLE);
        DrawText("[Paimon] Fondo: Tiro parabolico con gravedad", 850, 240, 16, ORANGE);

        EndDrawing();
    }

    // Descargar el lienzo de textura intermedio de la memoria de video
    UnloadRenderTexture(target3D);

    // Descargar ambos modelos de la gpu
    UnloadModel(modeloPaimon);
    UnloadModel(modeloLumine);

    CloseWindow();
    return 0;
}