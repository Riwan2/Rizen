//#include "zgame/gcomponent.h"
#include "rizen/rizen.h"
#include "zgame/zgame.h"

/*
    Init
*/

// 2D
FrameBuffer frame_buffer;
Square fbuffer_square;

// 3D
CameraTPS camera_tps;

// Model
Model model, model2, model_dragon, model_tree;

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
    init
*/

void init(App* app)
{
    // Frame buffer
    glm::vec2 d_size = Input::display_size();
    frame_buffer.init(d_size);
    fbuffer_square.set_origin(glm::vec2(0.5));
    fbuffer_square.set_pos(d_size * 0.5f);
    fbuffer_square.set_size(d_size * 0.8f);
    fbuffer_square.update_transform();

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
        Shader
    */

    app->ressource_manager()->add_shader("basic_2d", "2d/basic.vr", "2d/basic.fa");
    app->ressource_manager()->add_shader("basic", "basic.vr", "basic.fa");
    app->ressource_manager()->add_shader("basic_instanced", "inst_basic.vr", "inst_basic.fa");
    app->ressource_manager()->add_shader("map", "terrain/map.vr", "terrain/map.fa");
    
    /*
        Texture
    */

    app->ressource_manager()->add_texture_jpg("noise", "noise.jpg");
    app->ressource_manager()->add_texture_jpg("garden", "garden.jpg");
    app->ressource_manager()->add_texture_jpg("tree", "tree.jpg");

    /*
        Mesh
    */

    app->ressource_manager()->add_mesh("cube", "cube.obj");
    app->ressource_manager()->add_mesh("character", "character.obj");
    app->ressource_manager()->add_mesh("tree", "tree.obj");
    app->ressource_manager()->add_mesh("grass", "grass.obj");
    
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

    // Basic instanced
    {
        MaterialInfo info;
        info.color = glm::vec4(0, 0, 1, 1);
        info.ambient = 0.5;
        Shader* shader = app->ressource_manager()->shader("basic_instanced");
        app->ressource_manager()->add_material("basic_instanced", shader, info);
    }

    // Character
    {
        MaterialInfo info;
        info.color = glm::vec4(0.7, 0.9, 0.3, 1);
        info.ambient = 1.0;
        Shader* shader = app->ressource_manager()->shader("basic");
        app->ressource_manager()->add_material("character", shader, info);
    }

    // Map
    {
        MaterialInfo info;
        info.color = glm::vec4(40, 93, 33, 255);
        info.color /= 255.0f;
        info.ambient = 0.5;
        info.texture = app->ressource_manager()->texture("map");
        Shader* shader = app->ressource_manager()->shader("map");
        app->ressource_manager()->add_material("map", shader, info);
    }

    // Tree
    {
        MaterialInfo info;
        info.ambient = 0.9;
        info.texture = app->ressource_manager()->texture("tree");
        Shader* shader = app->ressource_manager()->shader("basic_instanced");
        app->ressource_manager()->add_material("tree", shader, info);
    }

    /*
        Map
    */
    
    int size = 200;
    map.init(app->ressource_manager()->material("map"), glm::vec2(size), glm::vec2(200), glm::vec3(-size / 2, 0, -size / 2));
    map.generate_random_heightmap(6, 0.3, 35);

    /*
        Model
    */

    app->ressource_manager()->add_model("cube", "cube", "basic");
    app->ressource_manager()->add_model("character", "character", "character");
    app->ressource_manager()->add_model("sheep", "cube", "basic_instanced", true);
    app->ressource_manager()->add_model("tree", "tree", "tree", true);

    /*
        Entity
    */

    player = input_move_blueprint(registry, app->ressource_manager()->model("character"));
    {
        auto transform = &registry.get<TransformComponent>(player);
       
        transform->set_position(glm::vec3(0, 0, 10));
        transform->set_scale(glm::vec3(1));
        transform->update();
    }
   
    for (int i = 0; i < 10; i++) {;
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
            transform->set_scale(glm::vec3(0.5, 1.0, 0.5));

            float radius = 10;

            int x = rand_float(-radius, radius);
            x = cos(x / radius) * x;
            int z = rand_float(-radius, radius);
            z = sin(z / radius) * z;

            double theta = 2 * PI * rand_float(0, 1);
            double r = sqrt(rand_float(0, 1));
            x =  r * radius * cos(theta);
            z =  r * radius * sin(theta);

            transform->set_position(glm::vec3(x, 0, z));
            transform->update();
        }
    }

    for (int i = 0; i < 20; i++) {
        auto entity = renderable_blueprint(registry, app->ressource_manager()->model("tree"));
        {
            auto transform = &registry.get<TransformComponent>(entity);
            transform->set_scale(glm::vec3(4));

            float x = rand_float(-size / 2, size / 2);
            float z = rand_float(-size / 2, size / 2);
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

/*
    Update
*/

void update(App* app)
{
    // resize window
    if (Input::on_resized()) {
        glm::vec2 d_size = Input::display_size();
        frame_buffer.resize(d_size);
        fbuffer_square.set_pos(d_size * 0.5f);
        fbuffer_square.set_size(d_size * 0.8f);
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

    // random map generation
    static bool active = true;

    if (Input::key_pressed(SDLK_RETURN))
        active = !active;

    ImGui::Begin("Terrain Generator", &active);
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

    // update system
    bounce_system.update(registry, &map);
    input_move_system.update(registry, &map);
    move_system.update(registry);

    // fps
    //std::cout << 1 / Time::delta() * 1000 << "\n";

    // update camera
    //camera_tps.move_angle_around(0.1 * Time::game_delta());
    
    //auto player_trans = registry.try_get<TransformComponent>(player);
    //auto player_move = registry.try_get<InputMoveComponent>(player);

    //if (player_move) {
    //    camera_tps.set_angle_around(-player_move->get_rotation() + 90);
    //}
    //if (player_trans) {
    //    camera_tps.set_target(player_trans->position());
    //}

    //if (Input::key_down(SDLK_UP))
    //    camera_tps.move_angle_y(0.5 * Time::game_delta());


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

    // camera_tps.set_position(cam_pos);
    // camera_tps.update_view();

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
    

    // // render 3D objects
    frame_buffer.bind();

    app->renderer()->begin(&camera_tps);
    app->clear(glm::vec4(0.95));
    
    render_system.render(app->renderer(), registry);
    map.render(app->renderer());

    app->renderer()->end();
    frame_buffer.unbind();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // // render frame buffer
    app->renderer_2d()->begin(Input::display_size(), app->ressource_manager()->shader("basic_2d"));
    app->renderer_2d()->render_square(fbuffer_square, frame_buffer.texture());
    app->renderer_2d()->end();
}

int main()
{
    App* app = new App();
    AppInfo info;
    info.size_x = 820;
    info.size_y = 620;
    app->init(info);

    init(app);

    while(!app->on_quit()) {
        app->begin();
        app->clear(glm::vec4(0.2));
        update(app);
        app->end();
    }

    delete app;
    return 0;
}