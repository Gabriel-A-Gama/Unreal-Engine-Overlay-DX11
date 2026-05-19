#pragma once
#include <vector>
#include "ImGui/imgui.h"

//#include "ImGui/imgui_internal.h"
struct Particle
{
    ImVec2 position;
    ImVec2 velocity;
    float life;
    float size;
    ImU32 color;
};

std::vector<Particle> particles;
const int MAX_PARTICLES = 100;

void InitializeParticles()
{
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        Particle particle;
        particle.position = ImVec2(rand() % (int)screen_size.x, rand() % (int)screen_size.y);
        particle.velocity = ImVec2((rand() % 20 - 10) / 10.0f, -(rand() % 20 / 100.0f + 0.2f));
        particle.life = 1.0f;
        particle.size = (rand() % 3 + 1) * 0.5f;
        particle.color = IM_COL32(255, 255, 255, 96);
        particles.push_back(particle);
    }
}

void UpdateParticles(float deltaTime)
{
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    for (int i = 0; i < particles.size(); ++i)
    {
        particles[i].position.x += particles[i].velocity.x * deltaTime * 60.0f;
        particles[i].position.y += particles[i].velocity.y * deltaTime * 60.0f;

        // Reposiciona as partículas se saírem da tela
        if (particles[i].position.y < 0)
        {
            particles[i].position.y = screen_size.y;
            particles[i].position.x = rand() % (int)screen_size.x;
        }
        if (particles[i].position.x < 0 || particles[i].position.x > screen_size.x)
        {
            particles[i].position.x = rand() % (int)screen_size.x;
        }
    }
}




void RenderParticles()
{
    for (int i = 0; i < particles.size(); ++i)
    {
        ImGui::GetWindowDrawList()->AddCircleFilled(particles[i].position, particles[i].size, particles[i].color);
    }
}
void render_background() {
    auto d = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos(); // Posição da janela atual
    ImVec2 windowSize = ImGui::GetWindowSize(); // Tamanho da janela atual

    ImVec2 center = ImVec2(
        windowPos.x + windowSize.x * 0.5f,  // Calcula o centro X
        windowPos.y + windowSize.y + 20      // Calcula o centro Y com deslocamento de 20 unidades para baixo
    );


    ImU32 color1 = ImColor(99, 12, 12, 225);  // 0.3f * 255 ≈ 76
    ImU32 color2 = ImColor(99, 12, 12, 175);  // 0.2f * 255 ≈ 51
    ImU32 color3 = ImColor(99, 12, 12, 150);  // 0.1f * 255 ≈ 25
    ImU32 color4 = ImColor(99, 12, 12, 100);  // 0.075f * 255 ≈ 19

    // Use the converted ImU32 colors in AddCircle
    d->AddCircle(center, 50, color1, 0, 2);
    d->AddCircle(center, 100, color2, 0, 2);
    d->AddCircle(center, 180, color3, 0, 2);
    d->AddCircle(center, 260, color4, 0, 2);

    //d->AddCircle(center, 50, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.3f)), 0, 1);
    //d->AddCircle(center, 100, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.2f)), 0, 1);
    //d->AddCircle(center, 180, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.1f)), 0, 1);
    //d->AddCircle(center, 260, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.075f)), 0, 1);


}
#include <cmath> // Para std::sin e std::cos

float animationOffset = 0.0f; // Variável para controlar a animação
float initialOffset = -200.0f; // Deslocamento inicial para começar mais à esquerda
float speed = 0.2f; // Velocidade da animação
float amplitude = 500.0f; // Amplitude do movimento, ajustado para ir mais além do centro
float direction = 1.0f; // Direção da animação: 1 para direita, -1 para esquerda

void RenderSkeletonBackground()
{
    auto d = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos(); // Posição da janela atual
    ImVec2 windowSize = ImGui::GetWindowSize(); // Tamanho da janela atual

    ImVec2 center = ImVec2(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);

    ImU32 lineColor = ImColor(0, 255, 0, 150); // Cor verde para o esqueleto
    ImU32 boxColor = ImColor(255, 0, 0, 150);  // Cor vermelha para a caixa contornada
    ImU32 textColor = ImColor(255, 255, 255, 255); // Cor branca para o texto
    ImU32 healthBarColor = ImColor(0, 255, 0, 150); // Cor verde para a barra de saúde

    // Atualizar o offset de animação
    float time = ImGui::GetTime(); // Tempo atual
    animationOffset = amplitude * std::sin(speed * time); // Cálculo do deslocamento horizontal

    // Definir os pontos do esqueleto (reduzido para metade do tamanho original)
    ImVec2 head = { center.x + initialOffset + animationOffset, center.y - 14 }; // Cabeça
    ImVec2 torsoTop = { center.x + initialOffset + animationOffset, center.y - 10 }; // Parte superior do torso
    ImVec2 torsoBottom = { center.x + initialOffset + animationOffset, center.y + 10 }; // Parte inferior do torso
    ImVec2 leftShoulder = { center.x + initialOffset + animationOffset - 10, center.y - 5 }; // Ombro esquerdo
    ImVec2 rightShoulder = { center.x + initialOffset + animationOffset + 10, center.y - 5 }; // Ombro direito
    ImVec2 leftHip = { center.x + initialOffset + animationOffset - 10, center.y + 20 }; // Quadril esquerdo
    ImVec2 rightHip = { center.x + initialOffset + animationOffset + 10, center.y + 20 }; // Quadril direito

    // Desenhar o esqueleto
    d->AddLine(head, torsoTop, lineColor, 1.0f); // Cabeça ao topo do torso
    d->AddLine(torsoTop, torsoBottom, lineColor, 1.0f); // Torso
    d->AddLine(torsoTop, leftShoulder, lineColor, 1.0f); // Ombro esquerdo
    d->AddLine(torsoTop, rightShoulder, lineColor, 1.0f); // Ombro direito
    d->AddLine(torsoBottom, leftHip, lineColor, 1.0f); // Quadril esquerdo
    d->AddLine(torsoBottom, rightHip, lineColor, 1.0f); // Quadril direito

    // Desenhar a caixa contornada (cornered box)
    ImVec2 topLeft = { center.x + initialOffset + animationOffset - 25, center.y - 35 };
    ImVec2 bottomRight = { center.x + initialOffset + animationOffset + 25, center.y + 35 };
    float width = bottomRight.x - topLeft.x;
    float height = bottomRight.y - topLeft.y;

    // Cálculo das coordenadas dos cantos
    ImVec2 topRight = { topLeft.x + width, topLeft.y };
    ImVec2 bottomLeft = { topLeft.x, topLeft.y + height };

    // Desenhar a caixa contornada com linhas divididas
    d->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x + width / 3, topLeft.y), boxColor, 1.f); // Top-left
    d->AddLine(ImVec2(topLeft.x + width * 2 / 3, topLeft.y), ImVec2(topRight.x, topRight.y), boxColor, 1.f); // Top-right
    d->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x, topLeft.y + height / 3), boxColor, 1.f); // Left-top
    d->AddLine(ImVec2(topLeft.x, topLeft.y + height * 2 / 3), ImVec2(topLeft.x, bottomLeft.y), boxColor, 1.f); // Left-bottom
    d->AddLine(ImVec2(topLeft.x, bottomLeft.y), ImVec2(topLeft.x + width / 3, bottomLeft.y), boxColor, 1.f); // Bottom-left
    d->AddLine(ImVec2(topLeft.x + width * 2 / 3, bottomLeft.y), ImVec2(topRight.x, bottomRight.y), boxColor, 1.f); // Bottom-right
    d->AddLine(ImVec2(topRight.x, topRight.y), ImVec2(topRight.x, topRight.y + height / 3), boxColor, 1.f); // Right-top
    d->AddLine(ImVec2(topRight.x, topRight.y + height * 2 / 3), ImVec2(topRight.x, bottomRight.y), boxColor, 1.f); // Right-bottom

    // Desenhar a barra de saúde ao lado da caixa
    float healthBarWidth = 4.0f; // Largura da barra de saúde
    float healthBarHeight = height; // Altura da barra de saúde
    float healthBarPadding = 5.0f; // Espaçamento entre a caixa e a barra de saúde

    ImVec2 healthBarTopLeft = { topRight.x + healthBarPadding, topRight.y };
    ImVec2 healthBarBottomRight = { healthBarTopLeft.x + healthBarWidth, bottomRight.y };

    // Desenhar a barra de saúde preenchida
    d->AddRectFilled(healthBarTopLeft, healthBarBottomRight, healthBarColor);

    // Desenhar o texto acima da caixa contornada e centralizado
    const char* name = "ws_devv"; // Nome a ser exibido
    ImVec2 textSize = ImGui::CalcTextSize(name); // Calcular o tamanho do texto
    ImVec2 textPos = { center.x + initialOffset + animationOffset - textSize.x / 2, topLeft.y - 20 }; // Posição centralizada e acima da caixa
    d->AddText(textPos, textColor, name); // Adicionar o texto

    // Atualizar a direção da animação
    float windowWidth = ImGui::GetWindowWidth(); // Largura da janela ImGui
    float maxOffset = amplitude; // Valor máximo do deslocamento

    if (animationOffset >= maxOffset || animationOffset <= -maxOffset) {
        direction *= -1; // Alternar a direção
    }
    animationOffset += speed * direction; // Atualizar o deslocamento
}

//#include <cmath> // Para std::sin e std::cos
//
//float animationOffset = 0.0f; // Variável para controlar a animação
//float initialOffset = -200.0f; // Deslocamento inicial para começar mais à esquerda
//
//void RenderSkeletonBackground()
//{
//    auto d = ImGui::GetWindowDrawList();
//    ImVec2 center = ImGui::GetCurrentWindow()->Rect().GetCenter();
//    ImU32 lineColor = ImColor(0, 255, 0, 150); // Cor verde para o esqueleto
//    ImU32 boxColor = ImColor(255, 0, 0, 150);  // Cor vermelha para a caixa contornada
//    ImU32 textColor = ImColor(255, 255, 255, 255); // Cor branca para o texto
//    ImU32 healthBarColor = ImColor(0, 255, 0, 150); // Cor verde para a barra de saúde
//
//    // Atualizar o offset de animação
//    float time = ImGui::GetTime(); // Tempo atual
//    float speed = 0.04f; // Velocidade da animação
//    float amplitude = 500.0f; // Amplitude do movimento
//    animationOffset = amplitude * std::sin(speed * time); // Cálculo do deslocamento horizontal
//
//    // Definir os pontos do esqueleto (reduzido para metade do tamanho original)
//    ImVec2 head = { center.x + initialOffset + animationOffset, center.y - 14 }; // Cabeça
//    ImVec2 torsoTop = { center.x + initialOffset + animationOffset, center.y - 10 }; // Parte superior do torso
//    ImVec2 torsoBottom = { center.x + initialOffset + animationOffset, center.y + 10 }; // Parte inferior do torso
//    ImVec2 leftShoulder = { center.x + initialOffset + animationOffset - 10, center.y - 5 }; // Ombro esquerdo
//    ImVec2 rightShoulder = { center.x + initialOffset + animationOffset + 10, center.y - 5 }; // Ombro direito
//    ImVec2 leftHip = { center.x + initialOffset + animationOffset - 10, center.y + 20 }; // Quadril esquerdo
//    ImVec2 rightHip = { center.x + initialOffset + animationOffset + 10, center.y + 20 }; // Quadril direito
//
//    // Desenhar o esqueleto
//    d->AddLine(head, torsoTop, lineColor, 1.0f); // Cabeça ao topo do torso
//    d->AddLine(torsoTop, torsoBottom, lineColor, 1.0f); // Torso
//    d->AddLine(torsoTop, leftShoulder, lineColor, 1.0f); // Ombro esquerdo
//    d->AddLine(torsoTop, rightShoulder, lineColor, 1.0f); // Ombro direito
//    d->AddLine(torsoBottom, leftHip, lineColor, 1.0f); // Quadril esquerdo
//    d->AddLine(torsoBottom, rightHip, lineColor, 1.0f); // Quadril direito
//
//    // Desenhar a caixa contornada (cornered box)
//    ImVec2 topLeft = { center.x + initialOffset + animationOffset - 25, center.y - 35 };
//    ImVec2 bottomRight = { center.x + initialOffset + animationOffset + 25, center.y + 35 };
//    float width = bottomRight.x - topLeft.x;
//    float height = bottomRight.y - topLeft.y;
//
//    // Cálculo das coordenadas dos cantos
//    ImVec2 topRight = { topLeft.x + width, topLeft.y };
//    ImVec2 bottomLeft = { topLeft.x, topLeft.y + height };
//
//    // Desenhar a caixa contornada com linhas divididas
//    d->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x + width / 3, topLeft.y), boxColor, 1.f); // Top-left
//    d->AddLine(ImVec2(topLeft.x + width * 2 / 3, topLeft.y), ImVec2(topRight.x, topRight.y), boxColor, 1.f); // Top-right
//    d->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x, topLeft.y + height / 3), boxColor, 1.f); // Left-top
//    d->AddLine(ImVec2(topLeft.x, topLeft.y + height * 2 / 3), ImVec2(topLeft.x, bottomLeft.y), boxColor, 1.f); // Left-bottom
//    d->AddLine(ImVec2(topLeft.x, bottomLeft.y), ImVec2(topLeft.x + width / 3, bottomLeft.y), boxColor, 1.f); // Bottom-left
//    d->AddLine(ImVec2(topLeft.x + width * 2 / 3, bottomLeft.y), ImVec2(topRight.x, bottomRight.y), boxColor, 1.f); // Bottom-right
//    d->AddLine(ImVec2(topRight.x, topRight.y), ImVec2(topRight.x, topRight.y + height / 3), boxColor, 1.f); // Right-top
//    d->AddLine(ImVec2(topRight.x, topRight.y + height * 2 / 3), ImVec2(topRight.x, bottomRight.y), boxColor, 1.f); // Right-bottom
//
//    // Desenhar a barra de saúde ao lado da caixa
//    float healthBarWidth = 4.0f; // Largura da barra de saúde
//    float healthBarHeight = height; // Altura da barra de saúde
//    float healthBarPadding = 5.0f; // Espaçamento entre a caixa e a barra de saúde
//
//    ImVec2 healthBarTopLeft = { topRight.x + healthBarPadding, topRight.y };
//    ImVec2 healthBarBottomRight = { healthBarTopLeft.x + healthBarWidth, bottomRight.y };
//
//    // Desenhar a barra de saúde preenchida
//    d->AddRectFilled(healthBarTopLeft, healthBarBottomRight, healthBarColor);
//
//    // Desenhar o texto acima da caixa contornada e centralizado
//    const char* name = "ws_devv"; // Nome a ser exibido
//    ImVec2 textSize = ImGui::CalcTextSize(name); // Calcular o tamanho do texto
//    ImVec2 textPos = { center.x + initialOffset + animationOffset - textSize.x / 2, topLeft.y - 20 }; // Posição centralizada e acima da caixa
//    d->AddText(textPos, textColor, name); // Adicionar o texto
//}
//
