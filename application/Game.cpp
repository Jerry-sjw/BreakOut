#include "Game.h"
#include "loader/Loader.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <sstream>
#include "PostProcessor.h"

using namespace irrklang;

std::unique_ptr<SpriteRenderer> Renderer;
std::unique_ptr<GameObject> Player;
std::unique_ptr<BallObject> Ball;
std::unique_ptr<ParticleGenerator> Particles;
std::unique_ptr<PostProcessor> Effects;
std::unique_ptr<TextRenderer> Text;
ISoundEngine* SoundEngine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_DEFAULT_OPTIONS);
ISound* breakout_sound = SoundEngine->play2D("./assets/audios/breakout.mp3", true, true, true, ESM_AUTO_DETECT, GL_FALSE);
ISound* bleep_player_sound = SoundEngine->play2D("./assets/audios/bleep.wav", true, true, true, ESM_AUTO_DETECT, GL_FALSE);
ISound* bleep_break_sound = SoundEngine->play2D("./assets/audios/bleep.mp3", true, true, true, ESM_AUTO_DETECT, GL_FALSE);
ISound* powerup_sound = SoundEngine->play2D("./assets/audios/powerup.wav", true, true, true, ESM_AUTO_DETECT, GL_FALSE);
ISound* solid_sound = SoundEngine->play2D("./assets/audios/solid.wav", true, true, true, ESM_AUTO_DETECT, GL_FALSE);

// �ڳ���������ʵ�ʱ�ͷ���Դ
void ReleaseSounds()
{
    if (breakout_sound) {
        breakout_sound->drop();  // �ֶ��ͷ���Ƶ����
        breakout_sound = nullptr;
    }
    if (bleep_player_sound) {
        bleep_player_sound->drop();
        bleep_player_sound = nullptr;
    }
    if (bleep_break_sound) {
        bleep_break_sound->drop();
        bleep_break_sound = nullptr;
    }
    if (powerup_sound) {
        powerup_sound->drop();
        powerup_sound = nullptr;
    }
    if (solid_sound) {
        solid_sound->drop();
        solid_sound = nullptr;
    }

    // ����ͷ���Ƶ����
    if (SoundEngine) {
        SoundEngine->drop();
        SoundEngine = nullptr;
    }
}
Game::Game(GLuint width, GLuint height)
	:State(GameState::GAME_MENU), Keys(), Width(width), Height(height){
}

Game::~Game() {

}

//��Ϸ��ʼ��
void Game::Init() {
    // ������ɫ��
	ResourceManager::LoadShader("./assets/shaders/sprite.vs", "./assets/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("./assets/shaders/particle.vs", "./assets/shaders/particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("./assets/shaders/post_processing.vs", "./assets/shaders/post_processing.fs", nullptr, "postprocessing");
    ResourceManager::LoadShader("./assets/shaders/text.vs", "./assets/shaders/text.fs", nullptr, "txtrender");
    // ������ɫ��
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f); //����ƽ��ͶӰ�����ע���޸�������ԭ��Ϊ���Ͻ�
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // ����ר������Ⱦ�Ŀ���
    Renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));
    // ��������
    ResourceManager::LoadTexture("./assets/textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("./assets/textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("./assets/textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("./assets/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("./assets/textures/paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("./assets/textures/particle.png", GL_TRUE, "particle");
    //�����������
    ResourceManager::LoadTexture("./assets/textures/powerup_speed.png", GL_TRUE, "speed");
    ResourceManager::LoadTexture("./assets/textures/powerup_passthrough.png", GL_TRUE, "passthrough");
    ResourceManager::LoadTexture("./assets/textures/powerup_sticky.png", GL_TRUE, "sticky");
    ResourceManager::LoadTexture("./assets/textures/powerup_increase.png", GL_TRUE, "increase");
    ResourceManager::LoadTexture("./assets/textures/powerup_confuse.png", GL_TRUE, "confuse");
    ResourceManager::LoadTexture("./assets/textures/powerup_chaos.png", GL_TRUE, "chaos");
    //���عؿ�
    GameLevel one; one.Load("./assets/levels/one.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("./assets/levels/two.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("./assets/levels/three.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("./assets/levels/four.lvl", this->Width, this->Height * 0.5);
    this->GameLevels.push_back(one);
    this->GameLevels.push_back(two);
    this->GameLevels.push_back(three);
    this->GameLevels.push_back(four);
    this->GameLevel_index = 0;                                                                                      
    //���õ�������
    glm::vec2 PlayerPos = glm::vec2(this->Width / 2 - 50, this->Height - 20);
    glm::vec2 PlayerSize = glm::vec2(100, 20);
    glm::vec3 PlayerColor = glm::vec3(0.8f, 0.8f, 0.7f);
    glm::vec2 PlayerVelocity = glm::vec2(PLAYER_VELOCITY, 0);
    Player = std::make_unique<GameObject>(PlayerPos, PlayerSize, ResourceManager::GetTexture("paddle"), PlayerColor, PlayerVelocity);
    //����������
    GLuint R = 10;
    glm::vec2 BallPos = glm::vec2(this->Width / 2 - R, this->Height - 20 - 2*R);
    glm::vec2 BallVelocity = glm::vec2(0, 0);
    Ball = std::make_unique<BallObject>(BallPos, R, BallVelocity, ResourceManager::GetTexture("face"));
    //��������ϵͳ
    Particles = std::make_unique<ParticleGenerator>(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        500
    );
    //������Ƶ��������
    SoundEngine->play2D("./assets/audios/breakout.mp3", GL_TRUE);
    //����֡������Ⱦ
    Effects = std::make_unique<PostProcessor>(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    Effects->Chaos = GL_FALSE;
    Effects->Confuse = GL_FALSE;
    Effects->Shake = GL_FALSE;
    //�����ı���Ⱦ��
    Text = std::make_unique<TextRenderer>(ResourceManager::GetShader("txtrender"), this->Width, this->Height);
    Text->Load("./assets/fonts/OCRAEXT.TTF", 24);
    //����������
    Lives = 3;
    glm::vec2 Ball_Lives_Pos = glm::vec2(0, 2 * R);
    for (int i = 0; i < Lives; i++) {
        Ball_Lives_Pos = glm::vec2(Ball_Lives_Pos.x + 2 * R, 2 * R);
        BallLives.push_back(std::make_unique<BallObject>(Ball_Lives_Pos, R - 4, glm::vec2(0, 0), ResourceManager::GetTexture("face")));
    }
    
}

//����������Ӧ
void Game::ProcessInput(GLfloat dt){
    if (this->State == GameState::GAME_MENU)
    {
        if (this->State == GameState::GAME_MENU)
        {
            if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
            {
                this->State = GameState::GAME_ACTIVATE;
                this->KeysProcessed[GLFW_KEY_ENTER] = true;
            }

            if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
            {
                this->GameLevel_index = (this->GameLevel_index + 1) % 4;
                this->KeysProcessed[GLFW_KEY_W] = true;
            }

            if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
            {
                if (this->GameLevel_index > 0)
                    --this->GameLevel_index;
                else
                    this->GameLevel_index = 3;
                this->KeysProcessed[GLFW_KEY_S] = true;
            }
        }

        //������Σ��ð����ɿ������ٴδ���
        for (int i = 0; i < 1024; ++i)
        {
            if (!this->Keys[i])
                this->KeysProcessed[i] = false;
        }
    }

    if (this->State == GameState::GAME_ACTIVATE) {
        if (this->Keys[GLFW_KEY_A] == 1) {
            if ((Player->Position.x - Player->Velocity.x * dt) > 0) {
                Player->Position.x = Player->Position.x - Player->Velocity.x * dt;
                if (Ball->Stuck) Ball->Position.x = Ball->Position.x - Player->Velocity.x * dt; //stuckʱ��Ҫ��͵���һ���ƶ�
            }
        }
        //ע��player���������Ͻǣ��ж��ұ߳�������ʱ��Ҫ����player����
        if (this->Keys[GLFW_KEY_D] == 1) {
            if ((Player->Position.x + Player->Velocity.x * dt) < (this->Width - Player->Size.x)) {
                Player->Position.x = Player->Position.x + Player->Velocity.x * dt;
                if (Ball->Stuck) Ball->Position.x = Ball->Position.x + Player->Velocity.x * dt;
            }
        } 
        //�ո����player����ball
        if (this->Keys[GLFW_KEY_SPACE] == 1) {
            Ball->Stuck = false;
            Ball->Velocity = glm::vec2(300, -220);
        }
    }
    if (this->Keys[GLFW_KEY_ESCAPE] == 1) {
        std::cout << "�ͷ�������Ƶ��Դ" << std::endl;
        // ֹͣ������Ƶ���ͷ���Դ
        ReleaseSounds();  // �ͷ���Ƶ���漰�����Դ
    }
}

//��Ϸ��ʱ�����
void Game::Update(GLfloat dt) {
    Ball.get()->move(dt, this->Width); //��ͱ߽���ײ
    this->DoCollision(); //���ש����ײ
    //����shake��Ч
    if (this->shake_time > 0) this->shake_time -= dt;
    else this->shake = 0;
    ResourceManager::GetShader("sprite").Use().SetInteger("shake", this->shake);
    ResourceManager::GetShader("sprite").Use().SetFloat("time", this->shake_time);
    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2)); //������Ч
    this->UpdatePowerUps(dt); //����ʵʱ����
    if (Ball->Position.y >= this->Height) // ��Ӵ��±߽����ùؿ�
    {
        if (Lives > 0) Lives--;
        else {
            Lives = 3;
            GameLevel_index = 0;
        }
        this->ResetLevel();
        this->ResetPlayer();
        //���������������
        Ball->PassThrough = GL_FALSE;
        Ball->Color = glm::vec3(1.0f);
        Ball->Sticky = GL_FALSE;
        Player->Color = glm::vec3(1.0f);
        PowerUps.clear();
        //������Ч������ɾ������Чָ�룬��Чδ����
        Effects->Chaos = GL_FALSE;
        Effects->Confuse = GL_FALSE;
        Effects->Shake = GL_FALSE;
    }
    if (this->State == GameState::GAME_ACTIVATE && this->GameLevels[this->GameLevel_index].IsCompleted())
    {
        if (GameLevel_index <= 2) {
            GameLevel_index++;
            this->ResetLevel();
            this->ResetPlayer();
            //���õ�������
            glm::vec2 PlayerPos = glm::vec2(this->Width / 2 - 50, this->Height - 20);
            glm::vec2 PlayerSize = glm::vec2(100, 20);
            glm::vec3 PlayerColor = glm::vec3(0.8f, 0.8f, 0.7f);
            glm::vec2 PlayerVelocity = glm::vec2(PLAYER_VELOCITY, 0);
            Player = std::make_unique<GameObject>(PlayerPos, PlayerSize, ResourceManager::GetTexture("paddle"), PlayerColor, PlayerVelocity);
            //����������
            GLuint R = 10;
            glm::vec2 BallPos = glm::vec2(this->Width / 2 - R, this->Height - 20 - 2 * R);
            glm::vec2 BallVelocity = glm::vec2(0, 0);
            Ball = std::make_unique<BallObject>(BallPos, R, BallVelocity, ResourceManager::GetTexture("face"));
        }
        else {
            this->State = GameState::GAME_WIN;
        }
    }
}

//��Ⱦ
void Game::Render() {
    //��Ϸ����
    if (this->State == GameState::GAME_ACTIVATE) {
        Effects->BeginRender();//��BeginRender��EndRender�ڼ�ʵ��֡�������Ⱦ������Ϊ����ת�Ƶ���ͨ�����У�Ϊ������׼��
            Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 45.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            this->GameLevels[this->GameLevel_index].Draw(Renderer.get());
            Player->Draw(Renderer.get());
            Ball->Draw(Renderer.get());
            for (int i = 0; i < Lives; i++) {
                BallLives[i]->Draw(Renderer.get());
            }
            for (const auto& powerUp : this->PowerUps)
                if (!powerUp->Destroyed)
                    powerUp->Draw(Renderer.get());
            if(!Ball->Stuck) Particles->Draw();
        Effects->EndRender();
        Effects->Render(glfwGetTime());//Render�ж��Ѿ���Ⱦ�õ���������������Ⱦ����Ļ��
        std::stringstream ss; ss << this->GameLevel_index;
        Text->RenderText("Level:" + ss.str(), this->Width / 2 - 50.f, 20.f, 1.0f);
    }
    if (this->State == GameState::GAME_MENU) // ��Ϸ��ʼǰ
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 45.0f, glm::vec3(0.0f, 0.0f, 0.0f));
        this->GameLevels[this->GameLevel_index].Draw(Renderer.get());
        Player->Draw(Renderer.get());
        Ball->Draw(Renderer.get());
        for (int i = 0; i < Lives; i++) {
            BallLives[i]->Draw(Renderer.get());
        }
        Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2 + 20.0f, 0.75f);
        std::stringstream ss; ss << this->GameLevel_index+1;
        Text->RenderText("Level:" + ss.str(), this->Width/2 - 50.f, this->Height / 2 - 20.f, 1.0f);
    }
    if (this->State == GameState::GAME_WIN) // ��Ϸ����
    {
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}

//aabb��Χ�м��
GLboolean Game::CheckCollision(GameObject* one, PowerUp* two) {
    if ((one->Position.x + one->Size.x > two->Position.x && one->Position.x < two->Position.x + two->Size.x) && (one->Position.y + one->Size.y > two->Position.y && one->Position.y < two->Position.y + two->Size.y)) {
        return GL_TRUE;
    }

    return GL_FALSE;
}

//aabbԲ���
Collision  Game::CheckCollision(GameObject* aabb, BallObject* ball) {
    glm::vec2 ball_center = ball->Position + ball->Radius;
    glm::vec2 aabb_half = glm::vec2(aabb->Size.x / 2, aabb->Size.y / 2);
    glm::vec2 aabb_center = aabb->Position + aabb_half;
    glm::vec2 dir = ball_center - aabb_center;
    glm::vec2 clamp = glm::clamp(dir, -aabb_half, aabb_half);
    glm::vec2 nearest = aabb_center + clamp;

    dir = nearest - ball_center;
    
    if(glm::length(dir) < ball->Radius) return std::make_tuple(GL_TRUE, this->VectorDirection(dir), dir);

    return std::make_tuple(GL_FALSE, Direction::UP, glm::vec2(0, 0));;
}

//��ײ��Ӧ
void Game::DoCollision() {
    for (GameObject& box : this->GameLevels[this->GameLevel_index].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(&box, Ball.get());
            if (std::get<0>(collision)) // ���collision �� true
            {   
                // ���ש�鲻��ʵ�ľ�����ש��,��ʵ��������shake��Ч
                if (!box.IsSolid)
                {
                    SoundEngine->play2D("./assets/audios/bleep.mp3", GL_FALSE);
                    box.Destroyed = GL_TRUE;
                    this->SpawnPowerUps(box);
                }
                if (box.IsSolid) {
                    SoundEngine->play2D("./assets/audios/solid.wav", GL_FALSE);
                    this->shake = 1;
                    this->shake_time = 0.05f;

                }
                else {
                    box.Destroyed = GL_TRUE;
                }
                // ��ײ����
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!Ball->PassThrough || (Ball->PassThrough && box.IsSolid)) { //������Ч�ж����������������߲���ײ��ֱ�Ӵ���
                    if (dir == Direction::LEFT || dir == Direction::RIGHT) // ˮƽ������ײ
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // ��תˮƽ�ٶ�
                        // �ض�λ
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == Direction::LEFT)
                            Ball->Position.x += penetration; // ��������
                        else
                            Ball->Position.x -= penetration; // ��������
                    }
                    else // ��ֱ������ײ
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // ��ת��ֱ�ٶ�
                        // �ض�λ
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == Direction::UP)
                            Ball->Position.y -= penetration; // ��������
                        else
                            Ball->Position.y += penetration; // ��������
                    }
                }
            }
        }

        // �͵������ײ
        Collision result = CheckCollision(Player.get(), Ball.get());
        if(!std::get<0>(result)) Ball->IsCollidingWithPlayer = false;
        if (!Ball->Stuck && std::get<0>(result))
        {   
            //SoundEngine->play2D("./assets/audios/bleep.wav", GL_FALSE);
            if (!Ball->IsCollidingWithPlayer) {
                SoundEngine->play2D("./assets/audios/bleep.wav", GL_FALSE);
                Ball->IsCollidingWithPlayer = true;  // ���Ϊ����ײ
            }
            if (Ball->Sticky) { //���ճ�������Ч����Ҫճס
                Ball->Stuck = GL_TRUE;
            }
            // x�����ٶȺ���ײ���������λ�ó����ȣ�������
            GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
            GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
            GLfloat percentage = distance / (Player->Size.x / 2);
            // ������ײ����
            GLfloat strength = 2.0f;
            glm::vec2 oldVelocity = Ball->Velocity;
            Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
            //Ball->Velocity.y = -Ball->Velocity.y;
            Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); 
            // ���۵�ǰ�ٶ��������Ǹ�����ǿ���� y �ٶ��ǡ������򡱣�Ҳ���ǳ��ϵ�������
            Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        }
    }

    for (std::unique_ptr<PowerUp>& powerUp : this->PowerUps)
    {
        if (!powerUp->Destroyed)
        {
            if (powerUp->Position.y >= this->Height)
                powerUp->Destroyed = GL_TRUE;
            if (CheckCollision(Player.get(), powerUp.get()))
            {   // �����뵲��Ӵ�ʱ������Ч
                ActivatePowerUp(powerUp.get());
                SoundEngine->play2D("./assets/audios/powerup.wav", GL_FALSE);
                powerUp->Destroyed = GL_TRUE;
                powerUp->Activated = GL_TRUE;
            }
        }
    }
}

//������ײ�ķ��򣬺�aabb��Χ�������߽Ӵ�
Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // ��
        glm::vec2(1.0f, 0.0f),  // ��
        glm::vec2(0.0f, -1.0f), // ��
        glm::vec2(-1.0f, 0.0f)  // ��
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

//���ùؿ�
void Game::ResetLevel()
{
    if (this->GameLevel_index == 0)this->GameLevels[0].Load("./assets/levels/one.lvl", this->Width, this->Height * 0.5f);
    else if (this->GameLevel_index == 1)
        this->GameLevels[1].Load("./assets/levels/two.lvl", this->Width, this->Height * 0.5f);
    else if (this->GameLevel_index == 2)
        this->GameLevels[2].Load("./assets/levels/three.lvl", this->Width, this->Height * 0.5f);
    else if (this->GameLevel_index == 3)
        this->GameLevels[3].Load("./assets/levels/four.lvl", this->Width, this->Height * 0.5f);
}

//���õ���
void Game::ResetPlayer()
{
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}


void Game::SpawnPowerUps(GameObject& block)
{
    if (ShouldSpawn(75)) // 1/75�ļ���
    {
        //�����������ǵ��߳���ʱ��
        std::unique_ptr<PowerUp> PowerUp_ptr = std::make_unique<PowerUp>("speed", glm::vec3(0.5f, 0.5f, 1.0f), 12.0f, block.Position, ResourceManager::GetTexture("speed"));
        this->PowerUps.push_back(std::move(PowerUp_ptr));
    }
    if (ShouldSpawn(75))
    {
        std::unique_ptr<PowerUp> PowerUp_ptr = std::make_unique<PowerUp>("sticky", glm::vec3(0.5f, 0.5f, 1.0f), 12.0f, block.Position, ResourceManager::GetTexture("sticky"));
        this->PowerUps.push_back(std::move(PowerUp_ptr));
    }
    if (ShouldSpawn(75))
    {
        std::unique_ptr<PowerUp> PowerUp_ptr = std::make_unique<PowerUp>("passthrough", glm::vec3(0.5f, 0.5f, 1.0f), 12.0f, block.Position, ResourceManager::GetTexture("passthrough"));
        this->PowerUps.push_back(std::move(PowerUp_ptr));
    }
    if (ShouldSpawn(75))
    {
        std::unique_ptr<PowerUp> PowerUp_ptr = std::make_unique<PowerUp>("increase", glm::vec3(0.5f, 0.5f, 1.0f), 12.0f, block.Position, ResourceManager::GetTexture("increase"));
        this->PowerUps.push_back(std::move(PowerUp_ptr));
    }
    if (ShouldSpawn(15)) // ������߱���Ƶ��������
    {
        std::unique_ptr<PowerUp> PowerUp_ptr = std::make_unique<PowerUp>("confuse", glm::vec3(0.5f, 0.5f, 1.0f), 6.0f, block.Position, ResourceManager::GetTexture("confuse"));
        this->PowerUps.push_back(std::move(PowerUp_ptr));
    }
    if (ShouldSpawn(15))
    {
        std::unique_ptr<PowerUp>PowerUp_ptr = std::make_unique<PowerUp>("chaos", glm::vec3(0.5f, 0.5f, 1.0f), 6.0f, block.Position, ResourceManager::GetTexture("chaos"));
        this->PowerUps.push_back(std::move(PowerUp_ptr));
    }

}

void Game::UpdatePowerUps(GLfloat dt)
{
    for (std::unique_ptr<PowerUp>& powerUp : this->PowerUps)
    {
        powerUp->Position += powerUp->Velocity * dt;
        if (powerUp->Activated)
        {
            powerUp->Duration -= dt;

            if (powerUp->Duration <= 0.0f)
            {
                // ֮��Ὣ��������Ƴ�
                powerUp->Activated = GL_FALSE;
                // ͣ��Ч��
                if (powerUp->Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {   // ����û������stickyЧ�����ڼ���״̬ʱ���ã�����ͬ��
                        Ball->Sticky = GL_FALSE;
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp->Type == "passthrough")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "passthrough"))
                    {
                        Ball->PassThrough = GL_FALSE;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp->Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {
                        Effects->Confuse = GL_FALSE;
                    }
                }
                else if (powerUp->Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {   
                        Effects->Chaos = GL_FALSE;
                    }
                }
                else if (powerUp->Type == "increase")
                {
                    Player->Size.x -= 50;
                }
                else if (powerUp->Type == "speed")
                {
                    Ball->Velocity *= 1/1.5;
                }
            }
        }
    }
    this->PowerUps.erase(
        std::remove_if(
            this->PowerUps.begin(),
            this->PowerUps.end(),
            [](const std::unique_ptr<PowerUp>& powerUp) {
                return powerUp->Destroyed && !powerUp->Activated; //�Ƴ����ݻ���û�м���ĵ���
            }
        ),
        this->PowerUps.end()
    );
}

void Game::ActivatePowerUp(PowerUp* powerUp)
{
    // ���ݵ������ͷ�������
    if (powerUp->Type == "speed")
    {
        Ball->Velocity *= 1.5;
    }
    else if (powerUp->Type == "sticky")
    {
        Ball->Sticky = GL_TRUE;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp->Type == "passthrough")
    {
        Ball->PassThrough = GL_TRUE;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp->Type == "increase")
    {
        Player->Size.x += 50;
    }
    else if (powerUp->Type == "confuse")
    {
        if (!Effects->Confuse)
            Effects->Confuse = GL_TRUE; // ֻ��chaosδ����ʱ��Ч��chaosͬ��
    }
    else if (powerUp->Type == "chaos")
    {
        if (!Effects->Chaos)
            Effects->Chaos = GL_TRUE;
    }
}
