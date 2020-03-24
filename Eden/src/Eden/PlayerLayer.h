#pragma once

#include "Eden/Core.h"
#include "Eden/Layer.h"
#include "Eden/Player.h"
#include "Eden/Streamers/StreamerCodes.h"

namespace Eden {
  // Implement playlist
  class EDEN_API PlayerLayer : public Layer
  {
  public:
    PlayerLayer(int PlayerName = EDEN_OPENAL_PLAYER);
    ~PlayerLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(Event& event) override;

    void Load(const std::string& song_name) { m_Player->Load(song_name); }
    void Play() { m_Player->Play(); }
    void Pause() { m_Player->Pause(); }
    void Stop() { m_Player->Stop(); }
  private:
    bool OnKeyPressed(KeyPressedEvent& e);

    int m_PlayerName;
    Player* m_Player;
  };

}
