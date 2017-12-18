using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameController : MonoBehaviour {

    public GameObject player;
    public GameObject computerPlayer;
    public GameObject endGameDisplay;
	public Camera endGameCamera;
	public Camera mainCamera;

	private bool isGameOver = false;

    void Awake()
    {
		endGameDisplay.SetActive (false);
		endGameCamera.enabled = false;
    }

	void Update()
	{
		if (!isGameOver) {
			if (player.tag == "dead") {
				GameOver ("You lose");
				return;
			}

			if (computerPlayer.tag == "dead") {
				GameOver ("You win");
				return;
			}
		}
	}

	void GameOver(string text) {
		endGameDisplay.SetActive (true);
		endGameDisplay.GetComponentInChildren<UnityEngine.UI.Text> ().text = text;
		endGameCamera.enabled = true;
		mainCamera.enabled = false;
		isGameOver = true;
	}
}
