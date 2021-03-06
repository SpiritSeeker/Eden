#include "edenpch.h"
#include "PlayerLayer.h"

#include "Eden/Streamers/OpenALPlayer.h"
#include "Eden/KeyCodes.h"
#include "Eden/Input.h"

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
        return true;
      }
      // Temporary. Replace with playlist
      case EDEN_KEY_LEFT:
      {
        if (Input::IsKeyPressed(EDEN_KEY_LEFT_CONTROL) || Input::IsKeyPressed(EDEN_KEY_RIGHT_CONTROL))
        {
          EDEN_TRACE("Going back by 5 seconds!");
          float position = m_Player->GetCurrentPosition() - 5;
          if (position < 0)
            position = 0;
          m_Player->SetCurrentPosition(position);
          return true;
        }

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
        return true;
      }

      case EDEN_KEY_RIGHT:
      {
        if (Input::IsKeyPressed(EDEN_KEY_LEFT_CONTROL) || Input::IsKeyPressed(EDEN_KEY_RIGHT_CONTROL))
        {
          EDEN_TRACE("Skipping ahead by 5 seconds!");
          float position = m_Player->GetCurrentPosition() + 5;
          // Handle switching to next song here.
          if (position > m_Player->GetSongDuration())
          {
            m_Player->Stop();
            return true;
          }
          m_Player->SetCurrentPosition(position);
          return true;
        }

        return true;
      }

      case EDEN_KEY_UP:
      {
        int current_volume = m_Player->GetVolume() + 5;
        if (current_volume > 100)
          current_volume = 100;
        m_Player->SetVolume(current_volume);
        return true;
      }

      case EDEN_KEY_DOWN:
      {
        int current_volume = m_Player->GetVolume() - 5;
        if (current_volume < 0)
          current_volume = 0;
        m_Player->SetVolume(current_volume);
        return true;
      }

      case EDEN_KEY_M:
      {
        if (m_Player->IsMuted())
          m_Player->UnMute();
        else
          m_Player->Mute();
        return true;
      }

      default:
        EDEN_WARN("Invalid shortcut: {0} ({1})", KeyCodeToString(e.GetKeyCode()), e.GetKeyCode());
    }

    return false;
  }

}
