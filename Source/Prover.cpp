#include "Prover.h"

bool Prover::started, Prover::theorem, Prover::finished;
std::thread* Prover::provingThread;
std::chrono::time_point<std::chrono::high_resolution_clock> Prover::startTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Prover::finishTime;

expr determinant(expr A1, expr B1, expr C1, expr A2, expr B2, expr C2, expr A3, expr B3, expr C3)
{
  /*
    A1 B1 C1
    A2 B2 C2
    A3 B3 C3
  */
  expr detA = B2 * C3 - C2 * B3;
  expr detB = C2 * A3 - A2 * C3;
  expr detC = A2 * B3 - B2 * A3;
  return A1 * detA + B1 * detB + C1 * detC;
}

bool proveCollinearity(expr A, expr B, expr C)
{
  expr A_conj = A.conj();
  expr B_conj = B.conj();
  expr C_conj = C.conj();
  /* matrix
    |A A~ 1|
    |B B~ 1| = 0?
    |C C~ 1|
  */
  expr detA = B * C_conj - B_conj * C;
  expr detB = C * A_conj - C_conj * A;
  expr detC = A * B_conj - A_conj * B;
  expr det = detA + detB + detC;
  det.print();
  std::cout << std::endl;
  expr expanded = det.expand();
  expanded.print();
  std::cout << std::endl;
  return expanded.checkZeroEquality();
}

bool proveConcurrency(expr A1, expr B1, expr C1, expr A2, expr B2, expr C2, expr A3, expr B3, expr C3)
{
  expr det = determinant(A1, B1, C1, A2, B2, C2, A3, B3, C3);
  det.print();
  std::cout << std::endl;
  expr expanded = det.expand();
  expanded.print();
  std::cout << std::endl;
  return expanded.checkZeroEquality();
}

bool proveOrthogonality(expr A1, expr B1, expr A2, expr B2)
{
  // some magic dunno how it works
  expr left = A1 * B2;
  expr right = A2 * B1;
  expr result = left + right;
  result.print();
  std::cout << std::endl;
  return result.expand().checkZeroEquality();
}

bool proveParallel(expr A1, expr B1, expr A2, expr B2)
{
  /*
    incompatible system
    zA1 + z~B1 = C1
    zA2 + z~B2 = C2
    therefore
    A1 * B2 - A2 * B1 = 0
  */
  expr left = A1 * B2;
  expr right = A2 * B1;
  expr result = left - right;
  result.print();
  std::cout << std::endl;
  return result.expand().checkZeroEquality();
}

bool proveEquivalence(expr A1, expr B1, expr A2, expr B2)
{
  expr A1_conj = A1.conj();
  expr B1_conj = B1.conj();
  expr A2_conj = A2.conj();
  expr B2_conj = B2.conj();
  expr left = (A1 - B1) * (A1_conj - B1_conj);
  expr right = (A2 - B2) * (A2_conj - B2_conj);
  expr result = left - right;
  result.print();
  std::cout << std::endl;
  return result.expand().checkZeroEquality();
}

bool proveEquivalence(expr A, expr B)
{
  expr diff = A - B;
  A.print();
  std::cout << "\n=\n";
  B.print();
  std::cout << std::endl;
  return diff.expand().checkZeroEquality();
}

bool proveInscription(expr A, expr B, expr C, expr D)
{
  /*
    cross-ratio
    (A - B) / (C - B)
    ----------------
    (A - D) / (C - D)
    must be real (zero imaginary part)
  */
  expr crossRatio = ((A - B) / (C - B)) / ((A - D) / (C - D));
  crossRatio.print();
  std::cout << std::endl;
  std::cout << crossRatio.getTEXformat() << std::endl;
  return (crossRatio - crossRatio.conj()).expand().checkZeroEquality();
}

bool Prover::getStarted()
{
  return started;
}

bool Prover::getFinished()
{
  return finished;
}

bool Prover::getTheorem()
{
  return theorem;
}

void Prover::proveInscription(Point* first, Point* second, Point* third, Point* fourth)
{
  if (started)
  {
    throw "Already proving";
  }
  started = true;

  ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(first->GetConstruction());
  ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(second->GetConstruction());
  ConstructionPoint* third_point_data = dynamic_cast<ConstructionPoint*>(third->GetConstruction());
  ConstructionPoint* fourth_point_data = dynamic_cast<ConstructionPoint*>(fourth->GetConstruction());

  expr A = first_point_data->coord;
  expr B = second_point_data->coord;
  expr C = third_point_data->coord;
  expr D = fourth_point_data->coord;

  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveInscription(A, B, C, D);
  finish();
  });
  provingThread->detach();
}

void Prover::start()
{
  started = true;
  startTime = std::chrono::high_resolution_clock::now();
}

void Prover::finish()
{
  finishTime = std::chrono::high_resolution_clock::now();
  finished = true;
  std::cout << theorem << std::endl;
}

void Prover::proveCollinearity(Point* first, Point* second, Point* third)
{
  if (started)
  {
    throw "Already proving";
  }
  ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(first->GetConstruction());
  ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(second->GetConstruction());
  ConstructionPoint* third_point_data = dynamic_cast<ConstructionPoint*>(third->GetConstruction());
  expr A = first_point_data->coord;
  expr B = second_point_data->coord;
  expr C = third_point_data->coord;
  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveCollinearity(A, B, C);
  finish();
  });
  provingThread->detach();
}

void Prover::proveConcurrency(Line* first, Line* second, Line* third)
{
  if (started)
  {
    throw "Already proving";
  }

  ConstructionLine* first_line_data = dynamic_cast<ConstructionLine*>(first->GetConstruction());
  ConstructionLine* second_line_data = dynamic_cast<ConstructionLine*>(second->GetConstruction());
  ConstructionLine* third_line_data = dynamic_cast<ConstructionLine*>(third->GetConstruction());
  expr zFirst = first_line_data->z_coef;
  expr zConjFirst = first_line_data->z_conj_coef;
  expr freeFirst = first_line_data->free_coef;
  expr zSecond = second_line_data->z_coef;
  expr zConjSecond = second_line_data->z_conj_coef;
  expr freeSecond = second_line_data->free_coef;
  expr zThird = third_line_data->z_coef;
  expr zConjThird = third_line_data->z_conj_coef;
  expr freeThird = third_line_data->free_coef;
  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveConcurrency(zFirst, zConjFirst, freeFirst, zSecond, zConjSecond, freeSecond, zThird, zConjThird, freeThird);
  finish();
  });
  provingThread->detach();
}

void Prover::updateTextBox()
{
  if (finished)
  {
    TextBox* provingState = dynamic_cast<TextBox*> (Drawer::dialogBox);
    if (provingState)
    {
      auto time = std::chrono::nanoseconds(finishTime - startTime);
      std::ostringstream str;
      str << std::fixed;
      str << std::setprecision(3);
      str << (double)(time.count()) / 1000000;
      if (theorem)
      {
        std::string text = "True. Time: " + str.str() + " ms";
        provingState->setText(text);
        provingState->setColor(Color(32, 133, 54));
      }
      else
      {
        std::string text = "False. Time: " + str.str() + " ms";
        provingState->setText(text);
        provingState->setColor(Color::Red);
      }
    }
  }
}

void Prover::proveConstruction(Point* first, Point* second)
{
  if (started)
  {
    throw "Already proving";
  }

  ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(first->GetConstruction());
  ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(second->GetConstruction());
  expr A = first_point_data->coord;
  expr B = second_point_data->coord;
  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveEquivalence(A, B);
  finish();
  });
  provingThread->detach();
}

void Prover::proveEquivalence(Point* first, Point* second, Point* third, Point* fourth)
{
  if (started)
  {
    throw "Already proving";
  }

  ConstructionPoint* first_point_data = dynamic_cast<ConstructionPoint*>(first->GetConstruction());
  ConstructionPoint* second_point_data = dynamic_cast<ConstructionPoint*>(second->GetConstruction());
  ConstructionPoint* third_point_data = dynamic_cast<ConstructionPoint*>(third->GetConstruction());
  ConstructionPoint* fourth_point_data = dynamic_cast<ConstructionPoint*>(fourth->GetConstruction());

  expr A = first_point_data->coord;
  expr B = second_point_data->coord;
  expr C = third_point_data->coord;
  expr D = fourth_point_data->coord;

  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveEquivalence(A, B, C, D);
  finish();
  });
  provingThread->detach();
}

void Prover::proveParallel(Line* first, Line* second)
{
  if (started)
  {
    throw "Already proving";
  }

  ConstructionLine* first_line_data = dynamic_cast<ConstructionLine*>(first->GetConstruction());
  ConstructionLine* second_line_data = dynamic_cast<ConstructionLine*>(second->GetConstruction());
  expr zFirst = first_line_data->z_coef;
  expr zConjFirst = first_line_data->z_conj_coef;
  expr zSecond = second_line_data->z_coef;
  expr zConjSecond = second_line_data->z_conj_coef;

  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveParallel(zFirst, zConjFirst, zSecond, zConjSecond);
  finish();
  });
  provingThread->detach();
}

void Prover::proveOrthogonality(Line* first, Line* second)
{
  if (started)
  {
    throw "Already proving";
  }

  ConstructionLine* first_line_data = dynamic_cast<ConstructionLine*>(first->GetConstruction());
  ConstructionLine* second_line_data = dynamic_cast<ConstructionLine*>(second->GetConstruction());
  expr zFirst = first_line_data->z_coef;
  expr zConjFirst = first_line_data->z_conj_coef;
  expr zSecond = second_line_data->z_coef;
  expr zConjSecond = second_line_data->z_conj_coef;

  provingThread = new std::thread(
    [=]()
  {
    start();
  Prover::theorem = ::proveOrthogonality(zFirst, zConjFirst, zSecond, zConjSecond);
  finish();
  });
  provingThread->detach();
}