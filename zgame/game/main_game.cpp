#include "main_game.h"

/*
    Init game
*/

// 2D
FrameBuffer frame_buffer;
FrameBuffer depth_frame_buffer;
Square fbuffer_square;

// 3D
CameraTPS camera_tps;
SkyBox skybox;

// System
RenderSystem render_system;
MoveSystem move_system;
InputMoveSystem input_move_system;
BounceSystem bounce_system;

// ECS
entt::registry registry;

// Entity
entt::entity player;

// Map
Map map;

/*
    Load ressource
*/

void MainGame::load(App* app)
{
    /*
        Shader
    */

    app->ressource_manager()->add_shader("basic_2d", "2d/basic.vr", "2d/basic.fa");
    app->ressource_manager()->add_shader("basic", "basic.vr", "basic.fa");
    app->ressource_manager()->add_shader("shadow", "shadow/shadow.vr", "shadow/shadow.fa");

    app->ressource_manager()->add_shader("map", "terrain/map.vr", "terrain/map.fa");
    app->ressource_manager()->add_shader("skybox", "terrain/skybox.vr", "terrain/skybox.fa");
    
    /*
        Texture
    */

    app->ressource_manager()->add_texture_jpg("noise", "noise.jpg");
    app->ressource_manager()->add_texture_jpg("garden", "garden.jpg");
    app->ressource_manager()->add_texture_jpg("tree", "tree.jpg");
    app->ressource_manager()->add_texture_jpg("sheep", "sheep.jpg");

    /*
        Mesh
    */

    app->ressource_manager()->add_mesh("cube", "cube.obj");
    app->ressource_manager()->add_mesh("character", "character.obj");
    app->ressource_manager()->add_mesh("tree", "tree.obj");
    app->ressource_manager()->add_mesh("grass", "grass.obj");
    app->ressource_manager()->add_mesh("sheep", "sheep.obj");
    
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
        info.texture = app->ressource_manager()->texture("noise");
        Shader* shader = app->ressource_manager()->shader("basic");
        app->ressource_manager()->add_material("basic", shader, info);
    }

    // Character
    {
        MaterialInfo info;
        info.color = glm::vec4(0.7, 0.9, 0.3, 1);
        info.ambient = 0.3;
        info.reflectivity = 0.0;
        Shader* shader = app->ressource_manager()->shader("basic");
        app->ressource_manager()->add_material("character", shader, info);
    }

    // Map
    {
        MaterialInfo info;
        info.color = glm::vec4(40, 93, 33, 255);
        info.color /= 255.0f;
        info.ambient = 0.3;
        info.texture = app->ressource_manager()->texture("map");
        Shader* shader = app->ressource_manager()->shader("map");
        app->ressource_manager()->add_material("map", shader, info);
    }

    // Tree
    {
        MaterialInfo info;
        info.ambient = 0.3;
        info.reflectivity = 0.0;
        info.texture = app->ressource_manager()->texture("tree");
        Shader* shader = app->ressource_manager()->shader("basic");
        app->ressource_manager()->add_material("tree", shader, info);
    }

    // Sheep
    {
        MaterialInfo info;
        info.ambient = 0.3;
        info.reflectivity = 0.0;
        info.texture = app->ressource_manager()->texture("sheep");
        Shader* shader = app->ressource_manager()->shader("basic");
        app->ressource_manager()->add_material("sheep", shader, info);
    }

    /*
        Model
    */

    app->ressource_manager()->add_model("cube", "cube", "basic");
    app->ressource_manager()->add_model("character", "character", "character", false);
    app->ressource_manager()->add_model("sheep", "sheep", "sheep", true);
    app->ressource_manager()->add_model("tree", "tree", "tree", true);
}

/*
    Init
*/

void MainGame::init(App* app)
{
    load(app);

    // Frame buffer
    glm::vec2 d_size = Input::display_size();
    frame_buffer.init(d_size);
    fbuffer_square.set_origin(glm::vec2(0.5));
    fbuffer_square.set_pos(d_size * 0.5f);
    //fbuffer_square.set_size(d_size * 0.8f);
    fbuffer_square.set_size(d_size * 1.0f);
    fbuffer_square.update_transform();

    depth_frame_buffer.init_depth(d_size);

    // camera
    camera_tps.init(Input::display_size());
    camera_tps.set_distance(100);
    camera_tps.set_angle_y(15);
    camera_tps.move_target(glm::vec3(0, 8, 0));
    camera_tps.set_target(glm::vec3(0, -15, 0));

    /*
        System
    */

    /*
        Map
    */
    
    int size = 400;
    map.init(app->ressource_manager()->material("map"), glm::vec2(size), glm::vec2(200), glm::vec3(-size / 2, 0, -size / 2));
    map.generate_random_heightmap(6, 0.3, 35);

    const std::string path = "cubemap/space/";
    std::vector<std::string> faces_name;
    faces_name.push_back(path + "right.png");
    faces_name.push_back(path + "left.png");
    faces_name.push_back(path + "top.png");
    faces_name.push_back(path + "bottom.png");
    faces_name.push_back(path + "front.png");
    faces_name.push_back(path + "back.png");
    skybox.init(app->ressource_manager()->shader("skybox"), faces_name);

    // Entities
    init_entities(app);
}

/*
    Entity
*/

void MainGame::init_entities(App* app)
{
    player = input_move_blueprint(registry, app->ressource_manager()->model("character"));
    {
        auto transform = &registry.get<TransformComponent>(player);
       
        transform->set_position(glm::vec3(0, 0, 10));
        transform->set_scale(glm::vec3(1));
        transform->update();
    }

    for (int i = 0; i < 4000; i++) {
        auto entity = bounce_blueprint(registry, app->ressource_manager()->model("sheep"));
        {
            auto move = &registry.get<MoveComponent>(entity);
            auto bounce = &registry.get<BounceComponent>(entity);
            auto transform = &registry.get<TransformComponent>(entity);

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

    std::vector<glm::vec2> last_positions;
    for (int i = 0; i < 20; i++) {
        auto entity = renderable_blueprint(registry, app->ressource_manager()->model("tree"));
        {
            auto transform = &registry.get<TransformComponent>(entity);
            transform->set_scale(glm::vec3(4));

            glm::vec2 map_pos, size;
            map_pos.x = map.position().x;
            map_pos.y = map.position().z;
            size = map.size() / 2.0f - glm::vec2(10.0f);
            float radius = 5.0f;

            glm::vec2 pos = random_position_no_overlap(map_pos, size, radius, last_positions);
            last_positions.push_back(pos);

            float x = pos.x;
            float z = pos.y;
            float y = map.get_heigth(glm::vec2(x, z));
            transform->set_position(glm::vec3(x, y, z));

            glm::vec3 terrain_norm = lerp(map.get_precise_normal(glm::vec2(x, z)), glm::vec3(0, 1, 0), 0.5);
            transform->set_up(terrain_norm);
        }
    }

    // for (int i = 0; i < 10000; i++) {
    //     auto entity = renderable_blueprint(registry, &model2);
    //     {
    //         TransformComponent* transform = &registry.get<TransformComponent>(entity);
    //         transform->set_scale(glm::vec3(0.2));

    //         float x = i % 10 + rand_float(-1, 1);
    //         float y = i / 600.0;
    //         float z = rand_float(-7, 7);
    //         transform->set_position(glm::vec3(x * 2 - 20, y - 10, z - 20));
    //         transform->update();
    //     }
    // }
}

void MainGame::update_imgui(App* app)
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f);
    // Random map generation
    ImGui::Begin("Terrain Generator");
    static glm::vec4 color = app->ressource_manager()->material("map")->color();
    ImGui::ColorEdit4("Color", glm::value_ptr(color));
    app->ressource_manager()->material("map")->set_color(color);

    static float octave = 6;
    static int amplitude = 35;
    static float roughness = 0.3f;

    ImGui::SliderFloat("Octave", &octave, 0, 20, NULL);
    ImGui::SliderInt("Amplitude", &amplitude, 5, 40);
    ImGui::SliderFloat("Roughness", &roughness, 0.001, 1.0);

    bool generate = ImGui::Button("Generate") | Input::key_pressed(SDLK_g);
    if (generate)
        map.generate_random_heightmap(octave, roughness, amplitude);

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
    ImGui::Text("Nb Entities: %d", (int)registry.size());
    ImGui::End();

    m_day_manager.imgui_update();

    ImGui::PopStyleVar();
}

/*
    Update game
*/

void MainGame::update(App* app)
{
    //imgui
    update_imgui(app);

    // resize window
    if (Input::on_resized()) {
        glm::vec2 d_size = Input::display_size();
        frame_buffer.resize(d_size);
        depth_frame_buffer.resize_depth(d_size);
        fbuffer_square.set_pos(d_size * 0.5f);
        fbuffer_square.set_size(d_size * 1.0f);
        fbuffer_square.update_transform();
        camera_tps.resize(d_size);
    }

    static bool polygon_mode = true;
    if (Input::key_pressed(SDLK_p))
        polygon_mode = !polygon_mode;

    if (polygon_mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // update system
    bounce_system.update(registry, &map);
    input_move_system.update(registry, &map);
    move_system.update(registry);
    
    //auto player_trans = registry.try_get<TransformComponent>(player);
    //auto player_move = registry.try_get<InputMoveComponent>(player);

    //if (player_move) {
    //    camera_tps.set_angle_around(-player_move->get_rotation() + 90);
    //}
    //if (player_trans) {
    //    camera_tps.set_target(player_trans->position());
    //}

    // set camera angle y with the controller
    //float controller_y = Input::right_controller_axis().y * Time::game_delta();

    //sif (controller_y > 0 && camera_tps.angle_y() < 50)
    //        camera_tps.move_angle_y(controller_y);

    //glm::vec3 cam_pos = camera_tps.position();
    //float map_h = map.get_heigth(glm::vec2(cam_pos.x, cam_pos.z));
    
    // if (cam_pos.y <= map_h + 2) {
    //     float final_angle = lerp(camera_tps.angle_y(), camera_tps.angle_y() + 0.5, 0.1);
    //     camera_tps.set_angle_y(final_angle);
    //     cam_pos.y = map_h + 2;

    // } else {
        
    //     if (Input::key_down(SDLK_DOWN))
    //         camera_tps.move_angle_y(-0.5 * Time::game_delta());

    //     if (controller_y < 0 && camera_tps.angle_y() > 20)
    //         camera_tps.move_angle_y(controller_y);
    // }

    /*
        Camera Movement
    */

    glm::vec2 mouse_scroll = Input::mouse_scroll() * (float)Time::game_delta();

    static float x_speed = 0;
    x_speed = lerp(x_speed, x_speed + mouse_scroll.x, 0.3);
    x_speed = lerp(x_speed, 0, 0.1);
    camera_tps.move_angle_around(x_speed);

    static float y_speed = 0;
    if (abs(mouse_scroll.x) < 1) {
        y_speed = lerp(y_speed, y_speed + mouse_scroll.y, 0.3);
        y_speed = lerp(y_speed, 0, 0.1);
        float distance = camera_tps.distance();
        float distance_max = 120;
        float factor = distance / distance_max + 0.2f;
        float dist_speed = y_speed * factor;

        //std::cout << distance << " speed: " << y_speed << " factor: " << factor << std::endl;
        camera_tps.move_distance(dist_speed);

        distance = camera_tps.distance();

        if (distance > distance_max)
            camera_tps.set_distance(lerp(distance, distance_max, 0.3));
        else if (distance < 0)
           camera_tps.set_distance(lerp(distance, 0, 0.3));
    }


    camera_tps.update();
    m_day_manager.update();

    render_system.begin(registry);


    Shader* shadow_shader = app->ressource_manager()->shader("shadow");
    glm::vec3 lightInvDir = m_day_manager.sun_direction() * 100.0f;

    float near_plane = 1.0f, far_plane = 1000.0f;
    float distance = camera_tps.distance() - 20.0f;
    glm::mat4 lightProjection = glm::ortho(-distance, distance, -distance, distance, near_plane, far_plane);  

    // Compute the MVP matrix from the light's point of view
    //float dist = 80;
    //glm::mat4 depthProjectionMatrix = glm::ortho<float>(-70, 70, -70, 70, 1, 180);

    glm::mat4 lightView = glm::lookAt(lightInvDir, 
                                  glm::vec3( 0.0f, 0.0f,  0.0f), 
                                  glm::vec3( 0.0f, 1.0f,  0.0f));  
    //glm::mat4 depthModelMatrix = glm::mat4(1.0);
    //glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

    glm::mat4 depthMVP = lightProjection * lightView;

    shadow_shader->bind();
    shadow_shader->set_mat4("projection_view", depthMVP);
    shadow_shader->set_mat4("bias", depthMVP);

    depth_frame_buffer.bind();

    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    //map.render_depth(shadow_shader, app->renderer());
    render_system.render_depth(shadow_shader, app->renderer());

    depth_frame_buffer.unbind();

    glCullFace(GL_BACK);



    app->ressource_manager()->shader("map")->bind();
    app->ressource_manager()->shader("map")->set_mat4("lightSpaceMatrix", depthMVP);
    
    app->ressource_manager()->shader("map")->set_int("shadowMap", 1);

    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depth_frame_buffer.texture()->texture_id());

    // // render 3D objects
    frame_buffer.bind();

    app->renderer()->begin(&camera_tps, m_day_manager.sun_direction(), m_day_manager.sky_color());
    app->clear(glm::vec4(0.95));
    skybox.render(&camera_tps, m_day_manager.night_factor());
    render_system.render(app->renderer());
    map.render(app->renderer());

    app->renderer()->end();
    frame_buffer.unbind();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



    render_system.end();




    glViewport(0, 0, Input::display_size().x, Input::display_size().y);

    ImGui::Begin("Depth");
    ImGui::Image((void*)(intptr_t)depth_frame_buffer.texture()->texture_id(), ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    // // render frame buffer
    app->renderer_2d()->begin(Input::display_size(), app->ressource_manager()->shader("basic_2d"));
    app->renderer_2d()->render_square(fbuffer_square, frame_buffer.texture());
    app->renderer_2d()->end();
}