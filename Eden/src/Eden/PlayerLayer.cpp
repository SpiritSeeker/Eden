#include "edenpch.h"
#include "PlayerLayer.h"

#include "Eden/Streamers/OpenALPlayer.h"
#include "Eden/KeyCodes.h"

namespace Eden {

  PlayerLayer::PlayerLayer(int PlayerName)
    : Layer("PlayerLayer") { m_PlayerName = PlayerName; }

  PlayerLayer::~PlayerLayer()
  {
    OnDetach();
  }

  void PlayerLayer::OnAttach()
  {
    switch (m_PlayerName) {
      case EDEN_OPENAL_PLAYER:
      {
        m_Player = new OpenALPlayer();
        break;
      }
      default:
        EDEN_ERROR("Invalid player name identifier: {0}", m_PlayerName);
    }
  }

  void PlayerLayer::OnDetach()
  {
    delete m_Player;
  }

  void PlayerLayer::OnUpdate()
  {

  }

  void PlayerLayer::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(EDEN_BIND_EVENT_FN(PlayerLayer::OnKeyPressed));
  }

  // Replace hard coding with user settings
  bool PlayerLayer::OnKeyPressed(KeyPressedEvent& e)
  {
    switch (e.GetKeyCode()) {
      case EDEN_KEY_SPACE:
      {
        if (m_Player->GetPlayerState() > PlayerEmpty)
        {
          if (m_Player->GetPlayerState() == PlayerPlay)
            m_Player->Pause();
          else
            m_Player->Play();
        }
        break;
      }
      // Temporary. Replace with playlist
      case EDEN_KEY_LEFT:
      {
        if (m_Player->GetCurrentPosition() > 2)
        {
          if (m_Player->GetPlayerState() == PlayerPlay)
          {
            m_Player->Stop();
            m_Player->Play();
          }
          else if (m_Player->GetPlayerState() == PlayerPause)
            m_Player->Stop();
        }
        break;
      }

      case EDEN_KEY_UP:
      {
        int current_volume = m_Player->GetVolume() + 5;
        if (current_volume > 100)
          current_volume = 100;
        m_Player->SetVolume(current_volume);
        break;
      }

      case EDEN_KEY_DOWN:
      {
        int current_volume = m_Player->GetVolume() - 5;
        if (current_volume < 0)
          current_volume = 0;
        m_Player->SetVolume(current_volume);
        break;
      }

      case EDEN_KEY_M:
      {
        if (m_Player->IsMuted())
          m_Player->UnMute();
        else
          m_Player->Mute();
        break;
      }

      default:
        EDEN_WARN("Invalid shortcut: {0} ({1})", (char)e.GetKeyCode(), e.GetKeyCode());
    }

    return true;
  }

}
