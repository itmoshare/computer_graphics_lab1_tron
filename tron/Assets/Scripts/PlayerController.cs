using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour {

    public enum Direction
    {
        Left,
        Right,
        Forward,
        Back
    }

    public class RotationInfo
    {
        float Degrees { get; set; }
        Vector3 Vector { get; set; }
    }

	public int speed;
	public Direction currentDirection; //0-Left, 1-Right, 2-Up, 3-Down
	public bool handleInput;
    public int maxSpeed;
    public int minSpeed;

    
	private Rigidbody rb;
    private float nextKeyHit;
    private bool isAlive = true;

	void Start ()
	{
		rb = GetComponent<Rigidbody>();
	}

	void Update() {
        if(isAlive)
        {
            var lastPosition = GetTrailPosition(transform.position, 0.2f);
            transform.Translate(Vector3.forward * speed * 0.005f);
            var newPosition = GetTrailPosition(transform.position, 0.2f);

            DrawCollidablePath(lastPosition, newPosition);

            if (handleInput)
                HandleInput();
        }
    }

    void DrawCollidablePath(Vector3 lastPos, Vector3 newPos)
    {
        GameObject colliderKeeper = new GameObject("collider");
        BoxCollider bc = colliderKeeper.AddComponent<BoxCollider>();
        colliderKeeper.transform.position = Vector3.Lerp(newPos, lastPos, 0.5f);
        colliderKeeper.transform.LookAt(newPos);
        bc.size = new Vector3(0.1f, 0.3f, Vector3.Distance(newPos, lastPos));
    }

    void FixedUpdate() {
       

	}

	void OnCollisionEnter(Collision col) {
        if (col.gameObject.name != "Ground")
        {
            this.speed = 0;
            this.isAlive = false;
        }

	}

	void HandleInput() {
        if (Time.time < nextKeyHit)
            return;
        nextKeyHit = Time.time + 0.1f;
		if(Input.GetKey(KeyCode.LeftArrow)) {
            MoveOrTurn(Direction.Left);
		}
		else if(Input.GetKey(KeyCode.RightArrow)) {
            MoveOrTurn(Direction.Right);
        }
		else if(Input.GetKey(KeyCode.UpArrow)) {
            MoveOrTurn(Direction.Forward);
		}
		else if(Input.GetKey(KeyCode.DownArrow)) {
            MoveOrTurn(Direction.Back);
		}
	}

	void MoveOrTurn(Direction direction)
	{
		if (currentDirection == direction) {
            if(speed < maxSpeed)
                speed++;
			return;
		}

        if(IsOpositeDirection(direction))
        {
            if(speed > minSpeed)
                speed--;
            return;
        }

        var vector3Direction = GetTurnDirectionVector(direction);
        transform.Rotate(vector3Direction);
        currentDirection = direction;
    }

    bool IsOpositeDirection(Direction direction)
	{
		switch (currentDirection)
        {
            case Direction.Left:
                return direction == Direction.Right;
            case Direction.Right:
                return direction == Direction.Left;
            case Direction.Forward:
                return direction == Direction.Back;
            case Direction.Back:
                return direction == Direction.Forward;
            default:
                return false;
        }
    }

    Vector3 GetCurrentDirectionVector()
    {
        return GetDirectionVector(currentDirection);
    }

    Vector3 GetDirectionVector(Direction direction)
    {

        return (Vector3)typeof(Vector3).GetProperty(direction.ToString().ToLower()).GetValue(null, null);
    }

    Vector3 GetTrailPosition(Vector3 position, float diff)
    {
        switch (currentDirection)
        {
            case Direction.Left:
                return new Vector3(position.x + diff, position.y, position.z);
            case Direction.Right:
                return new Vector3(position.x - diff, position.y, position.z);
            case Direction.Forward:
                return new Vector3(position.x, position.y, position.z - diff);
            case Direction.Back:
                return new Vector3(position.x, position.y, position.z + diff);
            default:
                return position;
        }
    }

        Vector3 GetTurnDirectionVector(Direction direction)
    {
        switch (currentDirection)
        {
            case Direction.Left:
                if (direction == Direction.Forward)
                    return new Vector3(0.0f, 90.0f, 0.0f);
                else
                    return new Vector3(0.0f, -90.0f, 0.0f);
            case Direction.Right:
                if (direction == Direction.Forward)
                    return new Vector3(0.0f, -90.0f, 0.0f);
                else
                    return new Vector3(0.0f, 90.0f, 0.0f);
            case Direction.Forward:
                if (direction == Direction.Left)
                    return new Vector3(0.0f, -90.0f, 0.0f);
                else
                    return new Vector3(0.0f, 90.0f, 0.0f);
            case Direction.Back:
                if (direction == Direction.Left)
                {
                    if (rb.rotation.eulerAngles.y == -180.0f)
                        return new Vector3(0.0f, 90.0f, 0.0f);
                    else // y == 180
                        return new Vector3(0.0f, -270.0f, 0.0f);
                }
                else
                {
                    if (rb.rotation.eulerAngles.y == -180.0f)
                        return new Vector3(0.0f, 270.0f, 0.0f);
                    else // y == 180
                        return new Vector3(0.0f, -90.0f, 0.0f);
                }
            default:
                throw new Exception();
        }

    }

   
}