//#include "zgame/gcomponent.h"
#include "rizen/rizen.h"
#include "zgame/zgame.h"

/*
    Init
*/

// 2D
Shader basic_shader2D;
FrameBuffer frame_buffer;
Square fbuffer_square;

// 3D
CameraTPS camera_tps;
Shader basic_shader, basic_inst_shader, map_shader;
Texture texture, map_texture;

// Model
Mesh cube, dragon;
Material material, material2, material_dragon, material_map;
Model model, model2, model_dragon;

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
    camera_tps.set_distance(150);
    camera_tps.set_angle_y(30);
    camera_tps.move_target(glm::vec3(0, 8, 0));

    /*
        System
    */

    /*
        Shader
    */

    basic_shader2D.init("2d/basic.vr", "2d/basic.fa");
    basic_shader.init("basic.vr", "basic.fa");
    basic_inst_shader.init("inst_basic.vr", "inst_basic.fa");
    map_shader.init("terrain/map.vr", "terrain/map.fa");
    
    /*
        Texture
    */

    texture.init_jpg("noise.jpg");
    map_texture.init_jpg("garden.jpg");

    /*
        Mesh
    */
    
    cube.init("cube.obj");
    dragon.init("character.obj");
    
    /* 
        Material
    */
    
    material.init(&basic_shader);
    material.set_color(glm::vec4(1.0, 0.0, 0.0, 0.0));
    material.set_ambient(0.7);
    material.set_reflectivity(1.0);
    material.set_shine_damper(4);
    material.set_texture(&texture);
    app->renderer()->bind_ubo(&material);

    material2.init(&basic_inst_shader);
    material2.set_color(glm::vec4(0, 0, 1, 0));
    material2.set_ambient(0.5);
    app->renderer()->bind_ubo(&material2);

    material_dragon.init(&basic_shader);
    material_dragon.set_color(glm::vec4(0.7, 0.9, 0.3, 1.0));
    material_dragon.set_ambient(1.0);
    app->renderer()->bind_ubo(&material_dragon);

    material_map.init(&map_shader);
    material_map.set_color(glm::vec4(0.3, 0.9, 0.4, 1.0));
    material_map.set_ambient(0.5);
    material_map.set_texture(&map_texture);
    app->renderer()->bind_ubo(&material_map);

    /*
        Map
    */
    
    int size = 300;
    map.init(&material_map, glm::vec2(size), glm::vec2(500), glm::vec3(-size / 2, 0, -size / 2));
    map.generate_random_heightmap(40, 100);

    /*
        Model
    */

    model.init(&cube, &material);
    model_dragon.init(&dragon, &material_dragon);
    model2.init_instanced(&cube, &material2);

    /*
        Entity
    */

    player = input_move_blueprint(registry, &model_dragon);
    {
        auto transform = &registry.get<TransformComponent>(player);
       
        transform->set_position(glm::vec3(0, 0, 10));
        transform->set_scale(glm::vec3(1));
        transform->update();
    }
   
    for (int i = 0; i < 500; i++) {;
        auto entity = bounce_blueprint(registry, &model);
        {
            auto move = &registry.get<MoveComponent>(entity);
            auto bounce = &registry.get<BounceComponent>(entity);
            auto transform = &registry.get<TransformComponent>(entity);

            //bounce->offset = i * 100;
            bounce->offset = rand_int(0, 1000);
            bounce->bounce_speed = rand_float(0.5, 0.8);
            bounce->speed = rand_float(0.1, 0.4);
            bounce->direction_timer.init(500);

            // transform
            transform->set_scale(glm::vec3(0.5, 1.0, 0.5));

            float radius = 50;

            int x = rand_float(-radius, radius);
            x = cos(x / radius) * x;
            int z = rand_float(-radius, radius);
            z = sin(z / radius) * z;

            double theta = 2 * PI * rand_float(0, 1);
            double r = sqrt(rand_float(0, 1));
            x =  r * radius * cos(theta),
            z =  r * radius * sin(theta);

            transform->set_position(glm::vec3(x, 0, z));
            transform->update();
        }
    }

    for (int i = 0; i < 10000; i++) {
        auto entity = renderable_blueprint(registry, &model2);
        {
            TransformComponent* transform = &registry.get<TransformComponent>(entity);
            transform->set_scale(glm::vec3(0.2));

            float x = i % 10 + rand_float(-1, 1);
            float y = i / 600.0;
            float z = rand_float(-7, 7);
            transform->set_position(glm::vec3(x * 2 - 20, y - 10, z - 20));
            transform->update();
        }
    }
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

    // reset map

    if (Input::key_pressed(SDLK_RETURN))
        map.reset_random();

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
        float distance_max = 200;
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
    app->clear(glm::vec4(0.4));
    
    render_system.render(app->renderer(), registry);
    map.render(app->renderer());

    app->renderer()->end();
    frame_buffer.unbind();

    // // render frame buffer
    app->renderer_2d()->begin(Input::display_size(), &basic_shader2D);
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