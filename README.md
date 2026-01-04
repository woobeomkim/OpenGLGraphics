# OpenGL Rendering Pipeline Study

> OpenGL의 기본 렌더링 파이프라인을 학습하기 위해 진행한 개인 학습 프로젝트입니다.  
> 정점 처리부터 화면 출력까지의 전체 흐름을 직접 구현하며 이해하는 데 목적이 있습니다.

---

## 프로젝트 개요

- **프로젝트 유형**: 그래픽스 학습 프로젝트
- **학습 주제**: OpenGL 기본 렌더링 파이프라인
- **개발 목적**:  
  OpenGL이 데이터를 어떻게 처리하여  
  화면에 최종 이미지를 출력하는지  
  전체 파이프라인 흐름을 이해하기 위한 프로젝트입니다.
- **개발 형태**: 개인 학습 프로젝트
- **개발 환경**: C++ / OpenGL
- **플랫폼**: PC

---

## 학습 목표

- OpenGL 렌더링 파이프라인의 단계별 역할 이해
- CPU → GPU 데이터 전달 흐름 이해
- 셰이더 기반 렌더링 구조 학습

---

## 구현 및 학습 내용

- OpenGL Context 생성 및 초기화
- VAO / VBO 를 이용한 정점 데이터 관리
- Vertex Shader / Fragment Shader 작성
- 좌표 변환 과정 이해  
  (Model → View → Projection)
- 기본 도형 렌더링 (Triangle / Quad)

---

## 렌더링 파이프라인 이해

- **Input Assembly**
- **Vertex Shader**
- **Primitive Assembly**
- **Rasterization**
- **Fragment Shader**
- **Framebuffer Output**

각 단계가 어떤 역할을 수행하며  
데이터가 어떻게 변환되는지를 중심으로 학습했습니다.

---

## 학습 결과 및 정리

- OpenGL은 상태 기반(State-Based) API이며  
  파이프라인 설정이 렌더링 결과에 직접적인 영향을 준다는 점
- 셰이더를 통해 GPU 동작을 명시적으로 제어할 수 있다는 점
- 렌더링 파이프라인에 대한 이해가  
  그래픽스 디버깅과 최적화의 기초가 된다는 점

---

## 한계 및 다음 학습 단계

- 본 프로젝트는 기본 파이프라인 학습을 목표로 하여  
  조명, 텍스처, 쉐도우 등 고급 기법은 포함하지 않았습니다.

**다음 학습 계획**
- Texture Mapping
- Basic Lighting Model (Phong / Blinn-Phong)
- Depth Test 및 Stencil Buffer
- 간단한 카메라 시스템 구현

---


## 참고 사항

- 본 프로젝트는 학습 목적으로 제작되었습니다.
