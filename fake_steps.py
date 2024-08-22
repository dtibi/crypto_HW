import requests
import os

# GitHub repository information
repo = os.getenv('GITHUB_REPOSITORY')
pull_request_sha = os.getenv('GITHUB_SHA')
github_token = os.getenv('GITHUB_TOKEN')

# Define 5 fake steps with different statuses
fake_steps = [
    {"name": "Step 1: Initialize", "status": "success"},
    {"name": "Step 2: Build", "status": "failure"},
    {"name": "Step 3: Test", "status": "neutral"},
    {"name": "Step 4: Deploy", "status": "cancelled"},
    {"name": "Step 5: Cleanup", "status": "success"}
]

# Headers for GitHub API request
headers = {
    'Authorization': f'token {github_token}',
    'Accept': 'application/vnd.github.v3+json'
}

# Create check runs for each fake step
for step in fake_steps:
    step_name = step["name"]
    step_status = step["status"]

    payload = {
        "name": step_name,
        "head_sha": pull_request_sha,
        "status": "completed",
        "conclusion": step_status,
        "output": {
            "title": step_name,
            "summary": f"Step {step_name} finished with status: {step_status}"
        }
    }

    response = requests.post(
        f"https://api.github.com/repos/{repo}/check-runs",
        headers=headers,
        json=payload
    )
    response.raise_for_status()

print("Fake steps completed.")