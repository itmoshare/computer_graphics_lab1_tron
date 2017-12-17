using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Collision2dController : MonoBehaviour {
	void OnCollisionEnter2D(Collision2D col) {
		print (gameObject.name + " : " + col.gameObject.name);
	}
}
