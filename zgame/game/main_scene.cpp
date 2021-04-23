#include "main_scene.h"

/*
    Init
*/

void MainScene::init(App* app)
{
    // Basic
    m_app = app;
    load();

    // Camera
    m_camera_tps.init(Input::display_size());
    m_camera_tps.set_distance(100);
    m_camera_tps.set_angle_y(15);
    m_camera_tps.move_target(glm::vec3(0, 8, 0));
    m_camera_tps.set_target(glm::vec3(0, -15, 0));

    // Map
    int size = 400;
    m_map.init(app->ressource_manager()->material("map"), glm::vec2(size), glm::vec2(200), glm::vec3(-size / 2, 0, -size / 2));
    m_map.generate_random_heightmap(6, 0.3, 35);

    // SkyBox
    const std::string path = "cubemap/space/";
    std::vector<std::string> faces_name;
    faces_name.push_back(path + "right.png");
    faces_name.push_back(path + "left.png");
    faces_name.push_back(path + "top.png");
    faces_name.push_back(path + "bottom.png");
    faces_name.push_back(path + "front.png");
    faces_name.push_back(path + "back.png");
    m_skybox.init(app->ressource_manager()->shader("skybox"), faces_name);

    // Entities
    init_entities();
}

/*
    Load
*/

void MainScene::load()
{
    /*
        Shader
    */

    ressource_manager()->add_shader("basic_2d", "2d/basic.vr", "2d/basic.fa");
    ressource_manager()->add_shader("basic", "basic.vr", "basic.fa");
    ressource_manager()->add_shader("shadow", "shadow/shadow.vr", "shadow/shadow.fa");
    ressource_manager()->add_shader("map", "terrain/map.vr", "terrain/map.fa");
    ressource_manager()->add_shader("skybox", "terrain/skybox.vr", "terrain/skybox.fa");
    
    /*
        Texture
    */

    ressource_manager()->add_texture_jpg("noise", "noise.jpg");
    ressource_manager()->add_texture_jpg("garden", "garden.jpg");
    ressource_manager()->add_texture_jpg("tree", "tree.jpg");
    ressource_manager()->add_texture_jpg("sheep", "sheep.jpg");

    /*
        Mesh
    */

    ressource_manager()->add_mesh("cube", "cube.obj");
    ressource_manager()->add_mesh("character", "character.obj");
    ressource_manager()->add_mesh("tree", "tree.obj");
    ressource_manager()->add_mesh("grass", "grass.obj");
    ressource_manager()->add_mesh("sheep", "sheep.obj");
    
    /* 
        Material
    */
    
    // Basic
    {
        MaterialInfo info;
        info.color = glm::vec4(1, 0, 0, 1);
        info.ambient = 0.7;
        info.reflectivity = 1.0;
        info.shine_damper = 4;
        info.texture = ressource_manager()->texture("noise");
        Shader* shader = ressource_manager()->shader("basic");
        ressource_manager()->add_material("basic", shader, info);
    }

    // Character
    {
        MaterialInfo info;
        info.color = glm::vec4(0.7, 0.9, 0.3, 1);
        info.ambient = 0.3;
        info.reflectivity = 0.0;
        Shader* shader = ressource_manager()->shader("basic");
        ressource_manager()->add_material("character", shader, info);
    }

    // Map
    {
        MaterialInfo info;
        info.color = glm::vec4(40, 93, 33, 255);
        info.color /= 255.0f;
        info.ambient = 0.3;
        info.texture = ressource_manager()->texture("map");
        Shader* shader = ressource_manager()->shader("map");
        ressource_manager()->add_material("map", shader, info);
    }

    // Tree
    {
        MaterialInfo info;
        info.ambient = 0.3;
        info.reflectivity = 0.0;
        info.texture = ressource_manager()->texture("tree");
        Shader* shader = ressource_manager()->shader("basic");
        ressource_manager()->add_material("tree", shader, info);
    }

    // Sheep
    {
        MaterialInfo info;
        info.ambient = 0.3;
        info.reflectivity = 0.0;
        info.texture = ressource_manager()->texture("sheep");
        Shader* shader = ressource_manager()->shader("basic");
        ressource_manager()->add_material("sheep", shader, info);
    }

    /*
        Model
    */

    ressource_manager()->add_model("cube", "cube", "basic", true);
    ressource_manager()->add_model("character", "character", "character", false);
    ressource_manager()->add_model("sheep", "sheep", "sheep", true);
    ressource_manager()->add_model("tree", "tree", "tree", true);
}

/*
    Init Entities
*/

void MainScene::init_entities()
{
    /*
        Player
    */

    m_player = input_move_blueprint(m_registry, ressource_manager()->model("character"));
    {
        auto transform = &m_registry.get<TransformComponent>(m_player);
       
        transform->set_position(glm::vec3(0, 0, 10));
        transform->set_scale(glm::vec3(1));
        transform->update();
    }

    /*
        Sheep
    */

    for (int i = 0; i < 400; i++) {
        auto entity = bounce_blueprint(m_registry, ressource_manager()->model("sheep"));
        {
            auto move = &m_registry.get<MoveComponent>(entity);
            auto bounce = &m_registry.get<BounceComponent>(entity);
            auto transform = &m_registry.get<TransformComponent>(entity);

            //bounce->offset = i * 100;
            bounce->offset = rand_int(0, 1000);
            bounce->bounce_speed = rand_float(0.5, 0.8);
            bounce->max_height = 2;
            bounce->speed = rand_float(0.1, 0.4);
            bounce->direction_timer.init(500);

            // transform
            float radius = 50;

            int x = rand_float(-radius, radius);
            x = cos(x / radius) * x;
            int z = rand_float(-radius, radius);
            z = sin(z / radius) * z;

            double theta = 2 * PI * rand_float(0, 1);
            double r = sqrt(rand_float(0, 1));
            x =  r * radius * cos(theta);
            z =  r * radius * sin(theta);

            transform->set_position(glm::vec3(x, 0, z));
        }
    }

    /*
        Tree
    */

    std::vector<glm::vec2> last_positions;
    for (int i = 0; i < 20; i++) {
        auto entity = renderable_blueprint(m_registry, ressource_manager()->model("tree"));
        {
            auto transform = &m_registry.get<TransformComponent>(entity);
            transform->set_scale(glm::vec3(4));

            glm::vec2 map_pos, size;
            map_pos.x = m_map.position().x;
            map_pos.y = m_map.position().z;
            size = m_map.size() / 2.0f - glm::vec2(10.0f);
            float radius = 5.0f;

            glm::vec2 pos = random_position_no_overlap(map_pos, size, radius, last_positions);
            last_positions.push_back(pos);

            float x = pos.x;
            float z = pos.y;
            float y = m_map.get_heigth(glm::vec2(x, z));
            transform->set_position(glm::vec3(x, y, z));

            glm::vec3 terrain_norm = lerp(m_map.get_precise_normal(glm::vec2(x, z)), glm::vec3(0, 1, 0), 0.5);
            transform->set_up(terrain_norm);
        }
    }

    /*
        Cubes
    */

    // for (int i = 0; i < 20000; i++) {
    //     auto entity = renderable_blueprint(m_registry, ressource_manager()->model("cube"));
    //     {
    //         TransformComponent* transform = &m_registry.get<TransformComponent>(entity);
    //         transform->set_scale(glm::vec3(0.2));

    //         float x = i % 10 + rand_float(-1, 1);
    //         float y = i / 600.0;
    //         float z = rand_float(-7, 7);
    //         transform->set_position(glm::vec3(x * 2 - 20, y - 10, z - 20));
    //         transform->update();
    //     }
    // }
}

/*
    Update
*/

void MainScene::update()
{
    m_bounce_system.update(m_registry, &m_map);
    m_input_move_system.update(m_registry, &m_map);
    m_move_system.update(m_registry);

    if (Input::on_resized()) {
        glm::vec2 d_size = Input::display_size();
        m_camera_tps.resize(d_size);
    }

    
    /*
        Camera Movement
    */

    glm::vec2 mouse_scroll = Input::mouse_scroll() * (float)Time::game_delta();

    static float x_speed = 0;
    x_speed = lerp(x_speed, x_speed + mouse_scroll.x, 0.3);
    x_speed = lerp(x_speed, 0, 0.1);
    m_camera_tps.move_angle_around(x_speed);

    static float y_speed = 0;
    if (abs(mouse_scroll.x) < 1) {
        y_speed = lerp(y_speed, y_speed + mouse_scroll.y, 0.3);
        y_speed = lerp(y_speed, 0, 0.1);
        float distance = m_camera_tps.distance();
        float distance_max = 120;
        float factor = distance / distance_max + 0.2f;
        float dist_speed = y_speed * factor;

        //std::cout << distance << " speed: " << y_speed << " factor: " << factor << std::endl;
        m_camera_tps.move_distance(dist_speed);

        distance = m_camera_tps.distance();

        if (distance > distance_max)
            m_camera_tps.set_distance(lerp(distance, distance_max, 0.3));
        else if (distance < 0)
           m_camera_tps.set_distance(lerp(distance, 0, 0.3));
    }

    m_camera_tps.update();
    m_day_manager.update();
}

/*
    Begin
*/

void MainScene::begin_render()
{
    m_render_system.begin(m_registry);
}

/*
    End
*/

void MainScene::end_render()
{
    m_render_system.end();
}

/*
    Render
*/

void MainScene::render()
{
    renderer()->begin(&m_camera_tps, m_day_manager.sun_direction(), m_day_manager.sky_color());
    m_app->clear(glm::vec4(0.95));

    m_skybox.render(&m_camera_tps, m_day_manager.night_factor());
    m_render_system.render(renderer());
    m_map.render(renderer());

    renderer()->end();
}

/*
    Render Depth
*/

void MainScene::render_depth(Shader* depth_shader)
{   
    m_render_system.render_depth(depth_shader, renderer());
    //m_map.render_depth(depth_shader, renderer());
}

/*
    Render Imgui
*/

void MainScene::update_imgui()
{
    // Random map generation
    ImGui::Begin("Terrain Generator");
    static glm::vec4 color = ressource_manager()->material("map")->color();
    ImGui::ColorEdit4("Color", glm::value_ptr(color));
    ressource_manager()->material("map")->set_color(color);

    static float octave = 6;
    static int amplitude = 35;
    static float roughness = 0.3f;

    ImGui::SliderFloat("Octave", &octave, 0, 20, NULL);
    ImGui::SliderInt("Amplitude", &amplitude, 5, 40);
    ImGui::SliderFloat("Roughness", &roughness, 0.001, 1.0);

    bool generate = ImGui::Button("Generate") | Input::key_pressed(SDLK_g);
    if (generate) {
        m_registry.clear();
        m_map.generate_random_heightmap(octave, roughness, amplitude);
        init_entities();
    }

    ImGui::End();


    // Performance
    const int size = 100;
    static float fps_arr[size];
    static int index = 0;
    fps_arr[index] = 1 / Time::delta() * 1000;

    if (index >= size - 1)
        for (int i = 1; i < size; i++)
            fps_arr[i-1] = fps_arr[i];
    else
        index++;

    ImGui::Begin("Performance");
    ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.5, 0.9, 0.6, 1.0));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1, 0.1, 0.2, 1.0));
    ImGui::PlotLines("Fps", fps_arr, IM_ARRAYSIZE(fps_arr), 0, NULL, 0.0f, 60.0f, ImVec2(0, 35));
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::Text("Nb Entities: %d", (int)m_registry.size());
    ImGui::End();

    m_day_manager.imgui_update();
}
