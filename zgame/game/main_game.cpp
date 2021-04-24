#include "main_game.h"

/*
    Init game
*/

// 2D
FrameBuffer frame_buffer;
FrameBuffer depth_frame_buffer;
Square fbuffer_square;

MainScene main_scene;

ShadowMap shadow_map;

/*
    Init
*/

void MainGame::init(App* app)
{
    app->ressource_manager()->add_shader("shadow", "shadow/shadow.vr", "shadow/shadow.fa");

    main_scene.init(app);
    
    // Frame buffer
    glm::vec2 d_size = Input::display_size();
    frame_buffer.init(d_size);
    fbuffer_square.set_origin(glm::vec2(0.5));
    fbuffer_square.set_pos(d_size * 0.5f);
    //fbuffer_square.set_size(d_size * 0.8f);
    fbuffer_square.set_size(d_size * 1.0f);
    fbuffer_square.update_transform();

    depth_frame_buffer.init_depth(d_size);

    shadow_map.init(app->ressource_manager()->shader("shadow"), d_size);
}

/*
    Update ImGui
*/

void MainGame::update_imgui(App* app)
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f);
    
    main_scene.update_imgui();
    shadow_map.update_imgui();

    ImGui::PopStyleVar();
}

/*
    Update game
*/

void MainGame::update(App* app)
{
    // Imgui
    update_imgui(app);

    // Resize window
    if (Input::on_resized()) {
        glm::vec2 d_size = Input::display_size();
        frame_buffer.resize(d_size);
        depth_frame_buffer.resize_depth(d_size * 2.0f);
        fbuffer_square.set_pos(d_size * 0.5f);
        fbuffer_square.set_size(d_size * 1.0f);
        fbuffer_square.update_transform();

        shadow_map.resize(d_size);
    }

    // Polygon Mode
    static bool polygon_mode = true;
    if (Input::key_pressed(SDLK_p))
        polygon_mode = !polygon_mode;

    if (polygon_mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Update
    main_scene.update();

    // Shader* shadow_shader = app->ressource_manager()->shader("shadow");
    // glm::vec3 lightInvDir = main_scene.day_manager()->sun_direction() * 100.0f;

    // float near_plane = 1.0f, far_plane = 1000.0f;
    // float distance = 120.0f;
    // //float distance = 50.0f;
    // glm::mat4 lightProjection = glm::ortho(-distance, distance, -distance, distance, near_plane, far_plane);  
    // glm::mat4 lightView = glm::lookAt(lightInvDir,
    //                               glm::vec3( 0.0f, 0.0f,  0.0f), 
    //                               glm::vec3( 0.0f, 1.0f,  0.0f));  
    // glm::mat4 depthMVP = lightProjection * lightView;

    //shadow_shader->bind();
    //shadow_shader->set_mat4("projection_view", depthMVP);

    // Render

    main_scene.begin_render();
    {
        glm::vec2 dsize = Input::display_size();
        // depth_frame_buffer.bind();
        
        // glViewport(0, 0, dsize.x * 2, dsize.y * 2);

        // glCullFace(GL_FRONT);
        // glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glClear(GL_DEPTH_BUFFER_BIT);

        shadow_map.begin_depth_render(main_scene.day_manager()->sun_direction());
        
        main_scene.render_depth(shadow_map.depth_shader());

        shadow_map.end_depth_render();

        //depth_frame_buffer.unbind();

        //glCullFace(GL_BACK);

        app->ressource_manager()->shader("map")->bind();
        app->ressource_manager()->shader("map")->set_mat4("lightSpaceMatrix", shadow_map.light_projection_view());
        app->ressource_manager()->shader("map")->set_int("shadowMap", 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_map.texture_id());

        // // render 3D objects
        frame_buffer.bind();

        glViewport(0, 0, dsize.x, dsize.y);

        main_scene.render();    
        frame_buffer.unbind();
    }
    main_scene.end_render();

    //glViewport(0, 0, Input::display_size().x, Input::display_size().y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // // render frame buffer
    app->renderer_2d()->begin(Input::display_size(), app->ressource_manager()->shader("basic_2d"));
    app->renderer_2d()->render_square(fbuffer_square, frame_buffer.texture());
    app->renderer_2d()->end();
}