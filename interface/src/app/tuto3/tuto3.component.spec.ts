import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { Tuto3Component } from './tuto3.component';

describe('Tuto3Component', () => {
  let component: Tuto3Component;
  let fixture: ComponentFixture<Tuto3Component>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ Tuto3Component ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(Tuto3Component);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
