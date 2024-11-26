package cz.cvut.fit.tjv.realestates.application;

import jakarta.persistence.EntityNotFoundException;

import java.util.List;

public interface Service<Entity, ID> {
    Entity getEntityById(ID id) throws EntityNotFoundException;
    List<Entity> getEntitiesByIds(List<ID> ids) throws EntityNotFoundException;
    List<Entity> getAllEntities();
    Entity createEntity(Entity entity) throws IllegalArgumentException;
    Entity updateEntity(Entity entity) throws EntityNotFoundException, IllegalArgumentException;
    void deleteEntity(ID id) throws EntityNotFoundException;
}
