<!-- omit in toc -->
# Contributing to OpenHaptics Firmware

First off, thanks for taking the time to contribute! ❤️

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways to help and details about how this project handles them. Please make sure to read the relevant section before making your contribution. It will make it a lot easier for us maintainers and smooth out the experience for all involved. The community looks forward to your contributions. 🎉

> And if you like the project, but just don't have time to contribute, that's fine. There are other easy ways to support the project and show your appreciation, which we would also be very happy about:
> - Star the project
> - Tweet about it
> - Refer this project in your project's readme
> - Mention the project at local meetups and tell your friends/colleagues
> - Sponsor the project

<!-- omit in toc -->
## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [I Have a Question](#i-have-a-question)
- [I Want To Contribute](#i-want-to-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Your First Code Contribution](#your-first-code-contribution)
    - [Making a pull request](#making-a-pull-request)
    - [Pull request reviews](#pull-request-reviews)
    - [Requirements](#requirements)
    - [Recommendations](#recommendations)
- [Join The Project Team](#join-the-project-team)

## Code of Conduct

This project and everyone participating in it is governed by the
[OpenHaptics Firmware Code of Conduct](./CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code. Please report unacceptable behavior
to [hello@leon0399.ru](mailto:hello@leon0399.ru).

## I Have a Question

> If you want to ask a question, we assume that you have read the available [Documentation](https://openhaptics.github.io/).

Before you ask a question, it is best to search for existing [Issues](https://github.com/openhaptics/openhaptics-firmware/issues) that might help you. In case you have found a suitable issue and still need clarification, you can write your question in this issue. It is also advisable to search the internet for answers first.

If you then still feel the need to ask a question and need clarification, we recommend the following:

- Open an [Issue](https://github.com/openhaptics/openhaptics-firmware/issues/new).
- Provide as much context as you can about what you're running into.
- Provide project and platform versions (nodejs, npm, etc), depending on what seems relevant.

We will then take care of the issue as soon as possible.

## I Want To Contribute

> ### Legal Notice <!-- omit in toc -->
> When contributing to this project, you must agree that you have authored 100% of the content, that you have the necessary rights to the content and that the content you contribute may be provided under the project license.

### Reporting Bugs

<!-- omit in toc -->
#### Before Submitting a Bug Report

A good bug report shouldn't leave others needing to chase you up for more information. Therefore, we ask you to investigate carefully, collect information and describe the issue in detail in your report. Please complete the following steps in advance to help us fix any potential bug as fast as possible.

- Make sure that you are using the latest version.
- Determine if your bug is really a bug and not an error on your side e.g. using incompatible environment components/versions (Make sure that you have read the [documentation](https://openhaptics.github.io/). If you are looking for support, you might want to check [this section](#i-have-a-question)).
- To see if other users have experienced (and potentially already solved) the same issue you are having, check if there is not already a bug report existing for your bug or error in the [bug tracker](https://github.com/openhaptics/openhaptics-firmwareissues?q=label%3Abug).
- Also make sure to search the internet (including Discord Server) to see if users outside of the GitHub community have discussed the issue.
- Collect information about the bug:
  - Stack trace (Traceback)
  - OS, Platform and Version (Windows, Linux, macOS, x86, ARM)
  - Version of the interpreter, compiler, SDK, runtime environment, package manager, depending on what seems relevant.
  - Possibly your input and the output
  - Can you reliably reproduce the issue? And can you also reproduce it with older versions?

<!-- omit in toc -->
#### How Do I Submit a Good Bug Report?

> You must never report security related issues, vulnerabilities or bugs including sensitive information to the issue tracker, or elsewhere in public. Instead sensitive bugs must be sent by email to [hello@leon0399.ru](mailto:hello@leon0399.ru).

We use GitHub issues to track bugs and errors. If you run into an issue with the project:

- Open an [Issue](https://github.com/openhaptics/openhaptics-firmware/issues/new). (Since we can't be sure at this point whether it is a bug or not, we ask you not to talk about a bug yet and not to label the issue.)
- Explain the behavior you would expect and the actual behavior.
- Please provide as much context as possible and describe the *reproduction steps* that someone else can follow to recreate the issue on their own. This usually includes your code. For good bug reports you should isolate the problem and create a reduced test case.
- Provide the information you collected in the previous section.

Once it's filed:

- The project team will label the issue accordingly.
- A team member will try to reproduce the issue with your provided steps. If there are no reproduction steps or no obvious way to reproduce the issue, the team will ask you for those steps and mark the issue as `needs-repro`. Bugs with the `needs-repro` tag will not be addressed until they are reproduced.
- If the team is able to reproduce the issue, it will be marked `needs-fix`, as well as possibly other tags (such as `critical`), and the issue will be left to be [implemented by someone](#your-first-code-contribution).'

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion for OpenHaptics Firmware, **including completely new features and minor improvements to existing functionality**. Following these guidelines will help maintainers and the community to understand your suggestion and find related suggestions.

<!-- omit in toc -->
#### Before Submitting an Enhancement

- Make sure that you are using the latest version.
- Read the [documentation](https://openhaptics.github.io/) carefully and find out if the functionality is already covered, maybe by an individual configuration.
- Perform a [search](https://github.com/openhaptics/openhaptics-firmware/issues) to see if the enhancement has already been suggested. If it has, add a comment to the existing issue instead of opening a new one.
- Find out whether your idea fits with the scope and aims of the project. It's up to you to make a strong case to convince the project's developers of the merits of this feature. Keep in mind that we want features that will be useful to the majority of our users and not just a small subset. If you're just targeting a minority of users, consider writing an add-on/plugin library.

<!-- omit in toc -->
#### How Do I Submit a Good Enhancement Suggestion?

Enhancement suggestions are tracked as [GitHub issues](https://github.com/openhaptics/openhaptics-firmware/issues).

- Use a **clear and descriptive title** for the issue to identify the suggestion.
- Provide a **step-by-step description of the suggested enhancement** in as many details as possible.
- **Describe the current behavior** and **explain which behavior you expected to see instead** and why. At this point you can also tell which alternatives do not work for you.
- **Explain why this enhancement would be useful** to most OpenHaptics Firmware users. You may also want to point out the other projects that solved it better and which could serve as inspiration.

### Your First Code Contribution

Code contributions must be submitted using [GitHub Pull Requests](https://github.com/openhaptics/openhaptics-firmware/pulls), where they will be reviewed by maintainers.

<!-- omit in toc -->
#### Useful documentation from github.com

- [About pull requests](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)
- [About forks](https://docs.github.com/en/github/collaborating-with-pull-requests/working-with-forks/about-forks)
- [Creating a pull request from a fork](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork)

<!-- omit in toc -->
#### Other things you'll need

- [git](https://git-scm.com/)

#### Making a pull request

The basic procedure to create a pull request is:
1. [Fork the repository on GitHub](https://github.com/openhaptics/openhaptics-firmware/fork). This gives you your own copy of the repository to make changes to.
2. Create a branch on your fork for your changes.
3. Make the changes you want to make on this branch.
4. You can then make a [pull request](https://github.com/openhaptics/openhaptics-firmware/pull/new) to the project.

#### Pull request reviews

Pull requests will be reviewed by maintainers when they are available.
Note that there might be a long wait time before a reviewer looks at your PR.

Depending on the changes, maintainers might ask you to make changes to the PR to fix problems or to improve the code.
**Do not delete your fork** while your pull request remains open, otherwise you won't be able to make any requested changes and the PR will end up being declined.

#### Requirements

The following are required as a minimum for pull requests. PRs that don't meet these requirements will be declined unless updated to meet them.

<!-- omit in toc -->
##### Licensing

OpenHaptics is licensed under [GPLv3 license](./LICENSE).
By proposing a pull request, you agree to your code being distributed within OpenHaptics Firmware under the same license.
If you take code from other projects, that code MUST be licensed under an LGPL-compatible license.

<!-- omit in toc -->
##### PRs should be about exactly ONE thing

If you want to make multiple changes, those changes should each be contributed as separate pull requests. **DO NOT** mix unrelated changes.

<!-- omit in toc -->
##### PRs must not include unnecessary/unrelated changes

Do not include changes which aren't strictly necessary. This makes it harder to review a PR, because the code diff becomes larger and harder to review.
This means:
- don't reformat or rearrange existing code
- don't change things that aren't related to the PR's objective
- don't rewrite existing code just to make it "look nicer"

<!-- omit in toc -->
##### Comments and documentation must be written in American English

English is the shared languages of all current maintainers.

#### Recommendations

- **Do not edit code directly on github.com.** We recommend learning how to use [`git`](https://git-scm.com). `git` allows you to "clone" a repository onto your computer, so that you can make changes using an IDE.
- **Create a new branch on your fork for each pull request.** This allows you to use the same fork to make multiple pull requests at the same time.
- **Use descriptive commit titles.** All pull requests SHOULD adhere to the [Conventional Commits specification](https://conventionalcommits.org/)
- **Do not include multiple unrelated changes in one commit.** An atomic style for commits is preferred - this means that changes included in a commit should be part of a single distinct change set. See [this link](https://www.freshconsulting.com/atomic-commits/) for more information on atomic commits. See the [documentation on `git add`](https://git-scm.com/docs/git-add) for information on how to isolate local changes for committing.
- **Your pull request will be checked and discussed in due time.** Since the team is scattered all around the world, your PR may not receive any attention for some time.
- **Do not copy-paste code**. There are potential license issues implicit with copy-pasting, and copy-paste usually indicates a lack of understanding of the actual code. Copy-pasted code is obvious a mile off and **any PR like this is likely to be closed**. If you want to use somebody else's code from a Git repository, **use [GIT's cherry-pick feature](https://git-scm.com/docs/git-cherry-pick)** to cherry-pick the commit.

## Join The Project Team

- Join our discord server: [Join](https://discord.gg/YUtRKAqty2)

[![Discord](https://img.shields.io/discord/966090258104062023?label=Discord&logo=discord)](https://discord.gg/YUtRKAqty2)

<!-- omit in toc -->
## Attribution

This guide is based on the **contributing-gen**. [Make your own](https://github.com/bttger/contributing-gen)!  
Parts of this guide are inspired by [PocketMine-MP guideline](https://github.com/pmmp/PocketMine-MP/blob/stable/CONTRIBUTING.md)
