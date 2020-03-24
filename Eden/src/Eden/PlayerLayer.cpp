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

      default:
        EDEN_WARN("Invalid shortcut: {0} ({1})", (char)e.GetKeyCode(), e.GetKeyCode());
    }

    return true;
  }

}
