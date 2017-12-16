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

	void Start ()
	{
		rb = GetComponent<Rigidbody>();
	}

	void Update() {

		transform.Translate(Vector3.forward * speed * 0.005f);
		if (handleInput)
			HandleInput();
    }

	void FixedUpdate() {
       

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
        switch (direction)
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